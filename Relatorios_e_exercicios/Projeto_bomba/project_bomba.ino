#include <avr/io.h>
#include <stdint.h>

#define LIGA (1 << PD2) // INT0
#define BUZZER (1 << PD4)
#define MOTOR (1 << PB1)
#define ADC_REF_VOLTAGE 5.0 // Reference voltage for ADC
#define ADC_RESOLUTION 1024 // ADC resolution (10-bit)
#define BAUD_RATE 115200
#define FLUXO_MAX 450

char msg_tx[20];
int volume = 0;
int tempo = 0;
float fluxoDesejado = 0;
int gotasAtual = 0;
int gotasTotal = 0;
int fluxoReal = 0;
int tempoGotas = 0;
int erro = 0;

unsigned int cont = 0;
unsigned char hora = 0, minuto = 0, segundo = 0;

// Função de configuração da UART
void UART_Init()
{
    // Configurar taxa de transmissão
    unsigned int baud_prescaler = F_CPU / (16 * BAUD_RATE) - 1;
    UBRR0H = (baud_prescaler >> 8) & 0xFF;
    UBRR0L = baud_prescaler & 0xFF;

    // Habilitar transmissão e recepção
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    // Configurar formato de frame: 8 bits de dados, 1 bit de parada
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

unsigned int UART_ReadInt()
{
    unsigned int value = 0;
    while (!(UCSR0A & (1 << RXC0)))
        ;               // Aguardar a recepção de dados
    value = UDR0 - '0'; // Converter o caractere recebido para um dígito numérico
    while (!(UCSR0A & (1 << RXC0)))
        ;                                // Aguardar a recepção de dados
    value = (value * 10) + (UDR0 - '0'); // Converter o segundo caractere recebido para um dígito numérico e multiplicar por 10
    while (!(UCSR0A & (1 << RXC0)))
        ;                                // Aguardar a recepção de dados
    value = (value * 10) + (UDR0 - '0'); // Converter o terceiro caractere recebido para um dígito numérico e multiplicar por 10
    return value;
}

// Função para transmitir um caractere pela UART
void UART_Transmit(unsigned char data)
{
    while (!(UCSR0A & (1 << UDRE0)))
        ;        // Aguardar o buffer de transmissão estar vazio
    UDR0 = data; // Transmitir o dado
    msg_tx[0] = 0;
}

// Função para transmitir uma string pela UART
void UART_TransmitString(const char *str)
{
    while (*str)
    {
        UART_Transmit(*str);
        str++;
    }
}

void UART_TrasmitNumber(int number)
{
    itoa(number, msg_tx, 10);
    UART_TransmitString(msg_tx);
}

// Função para verificar se há dados disponíveis na UART
bool UART_Available()
{
    return UCSR0A & (1 << RXC0);
}

// Função para ler um caractere da UART
unsigned char UART_Read()
{
    return UDR0;
}

int ADCsingleREAD(uint8_t channel)
{
    int ADCval;

    ADMUX = channel;        // use #1 ADC
    ADMUX |= (1 << REFS0);  // use AVcc as the reference
    ADMUX &= ~(1 << ADLAR); // clear for 10 bit resolution

    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // 128 prescale for 8Mhz
    ADCSRA |= (1 << ADEN);                                // Enable the ADC

    ADCSRA |= (1 << ADSC); // Start the ADC conversion

    while (ADCSRA & (1 << ADSC))
        ; // Thanks T, this line waits for the ADC to finish

    ADCval = ADCL;
    ADCval = (ADCH << 8) + ADCval; // ADCH is read so ADC can be updated again

    return ADCval;
}

void interruption_config()
{
    // Configurar o funcionamento da INT0 (Botão Liga no PD2)
    EICRA = (1 << ISC01) + (1 << ISC00);

    // Habilitar a INT0
    EIMSK = (1 << INT0);

    // Eu estou utilizando interrupção, se sim, DECLARA O "sei();"
    sei();
}

void PWM_Init()
{
    // Configurar o Timer/Counter1 no modo Fast PWM de 8 bits
    TCCR1A = (1 << COM1A1) | (1 << WGM10);
    TCCR1B = (1 << WGM12) | (1 << CS11); // Prescaler de 8 para o Timer/Counter1

    OCR1A = 0; // Inicializar o valor do registrador de comparação A
}

void timer_Init()
{
    // Configuração do Temporizador - Modo Comparador
    TCCR0A = (1 << WGM01);  // Configuração do modo de funcionamento para Comparador
    TCCR0B = (1 << CS01);   // Pre-scaler de 8 (Frequência de 2MHz - Período de 500 ns em cada contagem)
    OCR0A = 199;            // 200 contagens de 500 ns, o que gera uma interrupção a cada 100 us
    TIMSK0 = (1 << OCIE0A); // Gerar uma interrupção no estouro do comparador A
    sei();
}

void calcFluxoDesejado()
{
    fluxoDesejado = (volume / (tempo / 60.0));

    if (fluxoDesejado > FLUXO_MAX)
    {
        UART_TransmitString("Fluxo acima do maximo, definindo fluxo máximo\n");
        fluxoDesejado = FLUXO_MAX;
    }
    UART_TransmitString("Fluxo definido: ");
    UART_TrasmitNumber(fluxoDesejado);
    UART_TransmitString("\n");

    float potencia = fluxoDesejado * 100 / FLUXO_MAX;
    UART_TransmitString("Potencia: ");
    UART_TrasmitNumber(potencia);
    UART_TransmitString("%\n");

    OCR1A = (int)potencia;
}

void calcFluxoReal()
{
    fluxoReal = (gotasTotal / (tempoGotas / 60.0)) * 0.05;
    UART_TransmitString("Fluxo real: ");
    UART_TrasmitNumber(fluxoReal);
    UART_TransmitString("\n");

    erro = abs(((fluxoReal - fluxoDesejado) / fluxoDesejado)) * 100;
    UART_TransmitString("Erro: ");
    UART_TrasmitNumber(erro);
    UART_TransmitString("%\n");
}

int main()
{
    UART_Init();
    interruption_config();
    DDRB |= MOTOR;   // Declarar o LED como saída
    DDRD |= BUZZER;  // Declarar o BUZZER como saída
    PORTB &= ~MOTOR; // Começar com o LED desligado

    // PWM_Init(); // Inicializar o PWM

    while (1)
    {
        UART_TransmitString("UART Menu:\n");
        UART_TransmitString("1. Enter values\n");
        UART_TransmitString("2. Change values\n");
        UART_TransmitString("3. Continue\n");

        // Wait for user input
        while (!UART_Available())
            ;
        // Read the user's choice
        unsigned char choice = UART_Read();

        switch (choice)
        {
        case '1':
            UART_TransmitString("Entre com o Volume (0-999):\n");
            volume = UART_ReadInt();

            UART_TransmitString("Entre com o Tempo (0-999):\n");
            tempo = UART_ReadInt();

            UART_TransmitString("Valores cadastrados com Sucesso!\n");
            break;

        case '2':
            UART_TransmitString("Valores:\n");

            UART_TransmitString("volume: ");
            UART_TrasmitNumber(volume);
            UART_TransmitString("\n");

            UART_TransmitString("tempo: ");
            UART_TrasmitNumber(tempo);
            UART_TransmitString("\n");

            UART_TransmitString("Entre com o Volume (0-999):\n");
            volume = UART_ReadInt();

            UART_TransmitString("Entre com o Tempo (1-999):\n");
            tempo = UART_ReadInt();

            UART_TransmitString("Values changed successfully!\n");
            break;

        case '3':
            UART_TransmitString("Current values:\n");

            UART_TransmitString("volume: ");
            UART_TrasmitNumber(volume);
            UART_TransmitString("\n");

            UART_TransmitString("tempo: ");
            UART_TrasmitNumber(tempo);
            UART_TransmitString("\n");

            calcFluxoDesejado();
            calcFluxoReal();
            PORTB |= MOTOR;
            break;

        default:
            break;
        }
    }
}

ISR(INT0_vect)
{
    gotasAtual++;
    timer_Init();
    _delay_ms(100);

    // PORTB &= ~MOTOR;
}

ISR(TIMER0_COMPA_vect)
{
    int ADCvalue;
    cont++;

    if (cont == 10000)
    {
        if (gotasAtual > 0)
        {
            gotasTotal += gotasAtual;
            gotasAtual = 0;
            tempoGotas++;
        }

        ADCvalue = ADCsingleREAD(0);
        if (ADCvalue > 0)
        {
            PORTB &= ~MOTOR;
            PORTD |= BUZZER;
        }
        else
        {
            PORTD &= ~BUZZER;
        }
        // Convert ADC value to voltage
        float voltage = (float)ADCvalue * (ADC_REF_VOLTAGE / ADC_RESOLUTION);

        // Calculate percentage
        float percentage = (voltage / ADC_REF_VOLTAGE) * 100.0;
        // UART_TransmitString("PORCENTAGEM:");
        // itoa(percentage, msg_tx + strlen(msg_tx), 10);
        // UART_TransmitString(msg_tx);
        // UART_TransmitString("\n");
        cont = 0;
    }
}