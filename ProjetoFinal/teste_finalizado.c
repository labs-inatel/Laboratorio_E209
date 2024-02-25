#include <avr/io.h>
#include <stdint.h>

#define FOSC 16000000U // ClockSpeed
#define BAUD 9600
#define MYUBRR FOSC / 16 / BAUD - 1
#define LIGA (1 << PD2) // INT0
#define BUZZER (1 << PD4)
#define MOTOR (1 << PB1)
#define ADC_REF_VOLTAGE 5.0 // Reference voltage for ADC
#define ADC_RESOLUTION 1024 // ADC resolution (10-bit)
#define FLUXO_MAX 450

char msg_tx[30];
char msg_rx[32] = "\0";
int pos_msg_rx = 0;
int tamanho_msg_rx = 3;
int volume = 0;
int tempo = 0;
float fluxoDesejado = 0;
int gotasAtual = 0;
int gotasTotal = 0;
int fluxoReal = 0;
int tempoGotas = 0;
int erro = 0;

unsigned int cont = 0;

ISR(USART_RX_vect)
{
    // Escreve o valor recebido pela UART na posição pos_msg_rx do buffer msg_rx
    msg_rx[pos_msg_rx++] = UDR0;
    UART_Transmit("index: ");
    UART_TransmitNumber(pos_msg_rx - 1);
    UART_Transmit("\n");
    if (pos_msg_rx == tamanho_msg_rx) // Se o tamanho da mensagem recebida for igual ao tamanho máximo
        pos_msg_rx = 0;               // Reinicia a posição do buffer
}

void UART_Transmit(char *dados)
{
    // Envia todos os caracteres do buffer dados ate chegar um final de linha
    while (*dados != 0)
    {
        // Aguarda a transmissão acabar
        while (!(UCSR0A & (1 << UDRE0)))
            ;
        // Escreve o caractere no registro de tranmissão
        UDR0 = *dados;
        // Passa para o próximo caractere do buffer dados
        dados++;
    }
}
void UART_TransmitNumber(int number)
{
    char buffer[10];
    itoa(number, buffer, 10);
    UART_Transmit(buffer);
    // strcpy(msg_tx, "");
}

void UART_Init(unsigned int ubrr)
{
    // Configuraabaudrate
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    // Habilita a recepcao, tranmissao e interrupcao na recepcao
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
    // Configura o formato da mensagem: 8 bits de dados e 1bits de stop
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

int ADCsingleREAD(int channel)
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

void interruption_Init()
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
        UART_Transmit("Fluxo acima do maximo, definindo fluxo máximo\n");
        fluxoDesejado = FLUXO_MAX;
    }
    UART_Transmit("Fluxo definido: ");
    UART_TransmitNumber(fluxoDesejado);
    UART_Transmit("\n");

    float potencia = fluxoDesejado * 100 / FLUXO_MAX;
    UART_Transmit("Potencia: ");
    UART_TransmitNumber(potencia);
    UART_Transmit("%\n");

    OCR1A = (int)potencia;
}

void calcFluxoReal()
{
    fluxoReal = (gotasTotal / (tempoGotas / 60.0)) * 0.05;
    UART_Transmit("Fluxo real: ");
    UART_TransmitNumber(fluxoReal);
    UART_Transmit("\n");

    erro = abs(((fluxoReal - fluxoDesejado) / fluxoDesejado)) * 100;
    UART_Transmit("Erro: ");
    UART_TransmitNumber(erro);
    UART_Transmit("%\n");
}

int wait_For_Rx()
{
    while (strcmp(msg_rx, "\0") == 0)
        ; // Aguarda a recepção de uma mensagem
    return 1;
}

int main(void)
{
    UART_Init(MYUBRR);
    // PWM_Init();
    DDRB |= MOTOR;   // Declarar o LED como saída
    DDRD |= BUZZER;  // Declarar o BUZZER como saída
    PORTB &= ~MOTOR; // Começar com o LED desligado
    interruption_Init();
    sei();

    // Super-loop
    while (1)
    {

        UART_Transmit("UART Menu:\n");
        UART_Transmit("1. Entre com Valores\n");
        UART_Transmit("2. Mudar Valores\n");
        UART_Transmit("3. Continuar\n");
        wait_For_Rx();
        // se a mensagem recebida for um numero entre 0 e 100, altera o duty cycle do PWM
        int value = atoi(msg_rx); // converte a string recebida para inteiro

        if (msg_rx[0] != '\0')
        {
            strcat(msg_tx, "Valor recebido: ");
            UART_Transmit(msg_tx);
            UART_TransmitNumber(value);
            UART_Transmit("\n");
        }

        switch (value)
        {
        case 1:
            strcpy(msg_rx, "");
            UART_Transmit("Entre com o Volume (0-999): ");
            wait_For_Rx();
            volume = atoi(msg_rx); // converte a string recebida para inteiro
            UART_TransmitNumber(volume);
            UART_Transmit("\n");

            strcpy(msg_rx, "");
            UART_Transmit("Entre com o Tempo (0-999): ");
            wait_For_Rx();

            tempo = atoi(msg_rx); // converte a string recebida para inteiro
            UART_TransmitNumber(tempo);
            UART_Transmit("\n");

            strcpy(msg_rx, "");
            UART_Transmit("Valores cadastrados com Sucesso!\n");
            break;

        case 2:
            msg_rx[0] = '\0';
            UART_Transmit("Valores:\n");

            UART_Transmit("volume: ");
            UART_TransmitNumber(volume);
            UART_Transmit("\n");

            UART_Transmit("tempo: ");
            UART_TransmitNumber(tempo);
            UART_Transmit("\n");

            strcpy(msg_rx, "");
            UART_Transmit("Entre com o Volume (0-999): ");
            wait_For_Rx();
            volume = atoi(msg_rx); // converte a string recebida para inteiro
            UART_TransmitNumber(volume);
            UART_Transmit("\n");

            strcpy(msg_rx, "");
            UART_Transmit("Entre com o Tempo (0-999): ");
            wait_For_Rx();

            tempo = atoi(msg_rx); // converte a string recebida para inteiro
            UART_TransmitNumber(tempo);
            UART_Transmit("\n");

            strcpy(msg_rx, "");
            UART_Transmit("Valores cadastrados com Sucesso!\n");

        case 3:
            strcpy(msg_rx, "");
            UART_Transmit("Valores:\n");

            UART_Transmit("volume: ");
            UART_TransmitNumber(volume);
            UART_Transmit("\n");

            UART_Transmit("tempo: ");
            UART_TransmitNumber(tempo);
            UART_Transmit("\n");

            calcFluxoDesejado();
            calcFluxoReal();
            PORTB |= MOTOR;
            break;

        default:
            break;
        }
        strcpy(msg_rx, "");
    }

    strcpy(msg_rx, "");
}

ISR(INT0_vect)
{
    gotasAtual++;
    timer_Init();

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