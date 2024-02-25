#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#define BTN_CLICK (1 << PB1)

#define FOSC 16000000U // ClockSpeed
#define BAUD 9600
#define MYUBRR FOSC / 16 / BAUD - 1
#define MAX_BUFFER_SIZE 32

typedef struct {
    uint16_t value;
    float voltage;
} AnalogData;

volatile unsigned int timer2_cont;
volatile unsigned int timer2_segundos;
volatile bool UART_StringReceived = false;
volatile char buffer[MAX_BUFFER_SIZE];
volatile uint8_t bufferIndex = 0;

void timer2_callback(char *message)
{
    TCCR2A = (1 << WGM21);    // Configuração do modo de funcionamento para Comparador
    TCCR2B = (1 << CS21);     // Pre-scaler de 8 (Frequência de 2MHz - Período de 500 ns em cada contagem)
    OCR2A = 199;              // 200 contagens de 500 ns, o que gera uma interrupção a cada 100 us
}

void TIMER2_setup(void (*callback)(char *))
{
    TCCR2A = (1 << WGM21);
    TCCR2B = (1 << CS21);
    OCR2A = 199;
    TIMSK2 = (1 << OCIE2A);
}

unsigned int TIMER2_get_segundos()
{
    return timer2_segundos;
}

unsigned int TIMER2_get_cont()
{
    return timer2_cont;
}

void UART_Init(unsigned int ubrr)
{
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

    memset((void *)buffer, 0, sizeof(buffer));
}

void UART_Transmit(char *dados)
{
    while (*dados != 0)
    {
        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 = *dados;
        dados++;
    }
}

ISR(TIMER2_COMPA_vect)
{
    timer2_cont++;

    if (timer2_cont == (1000 * 10))
    {
        timer2_cont = 0;
        timer2_segundos++;

        char message[20];
        sprintf(message, "Segundos: %d\n", timer2_segundos);
        timer2_callback(message);
    }
}

ISR(USART_RX_vect)
{
    if (!UART_StringReceived)
    {
        buffer[bufferIndex] = UDR0;
        if (buffer[bufferIndex] == '\n')
        {
            UART_StringReceived = true;
            buffer[bufferIndex] = '\0';
            bufferIndex = 0;
        }
        else
        {
            bufferIndex++;
        }
    }
}

int ADC_read(uint8_t ch)
{
    char i;
    int ADC_temp = 0;
    int ADC_read = 0;
    ch &= 0x07;

    ADMUX = (ADMUX & 0xF8) | ch;
    ADCSRA |= (1 << ADSC);
    while (!(ADCSRA & (1 << ADIF)));

    for (i = 0; i < 8; i++)
    {
        ADCSRA |= (1 << ADSC);
        while (!(ADCSRA & (1 << ADIF)));
        ADC_temp = ADCL;
        ADC_temp += (ADCH << 8);
        ADC_read += ADC_temp;
    }
    ADC_read = ADC_read >> 3;
    return ADC_read;
}

AnalogData get_from_adc(uint8_t ch)
{
    uint16_t adc_result = ADC_read(ch);
    float voltage = (adc_result * 5.0) / 1023;
    AnalogData adc_struct = {adc_result, voltage};
    return adc_struct;
}

char *UART_GetString()
{
    UART_StringReceived = false;

    return (char *)buffer;
}

int main(void)
{
    UART_Init(MYUBRR);

    TIMER2_setup(timer2_callback);

    PCICR |= (1 << PCIE0);
    PCMSK0 |= (1 << PCINT1) | (1 << PCINT2);

    sei();

    while (1)
    {
        if (UART_StringReceived)
        {
            char *message = UART_GetString();
            unsigned char input = rx_buffer[i];
          
            UART_Transmit("Entre com o Volume: ");
            if (strcmp(message, "on") == 0)
            {
                UART_Transmit("ligando\n");
                TIMSK2 = (1 << OCIE2A);
            }

            if (strcmp(message, "off") == 0)
            {
                UART_Transmit("desligando\n");
                TIMSK2 = (0 << OCIE2A);
                timer2_segundos = 0;
                timer2_cont = 0;
            }
            UART_Transmit(strcat(message, "\n"));
        }
    }

    return 0;
}  
  