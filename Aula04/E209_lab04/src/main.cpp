#include <Arduino.h>

#define EXAMPLE_CODE 1
#define QUESTION_1_B 2
#define QUESTION_1_C 3
#define QUESTION_2 4

// MUDAR AQUI PARA TESTAR OS CÓDIGOS
#define WHAT_CODE QUESTION_1_C

#if WHAT_CODE == EXAMPLE_CODE

int main()
{
  DDRD = DDRD | 0b10000000;      // Pino PD7 definido como saída
  PORTD = PORTD | 0b00100000;    // Habilitar PULL-UP no PD5
  PORTD = PORTD & ~(0b10000000); // Desliga a saída PD7

  for (;;) // Super Loop
  {
    int botao = PIND & 0b00100000; // Lê o estado do PD5
    if (botao == 0)                // Botão está pressionado ?
    {
      PORTD = PORTD | 0b10000000;    // PD7 -> HIGH
      _delay_ms(1000);               // Espera 5s
      PORTD = PORTD & ~(0b10000000); // PD7 -> LOW
    }
  }

  return 0;
}

#elif WHAT_CODE == QUESTION_1_B

int main()
{
  DDRD = DDRD | 0b11000000;      // Pino PD7 e PD6 definido como saída
  PORTD = PORTD | 0b00110000;    // Habilitar PULL-UP no PD5 e PD4
  PORTD = PORTD & ~(0b11000000); // Desliga a saída PD7 e PD6

  for (;;) // Super Loop
  {
    int botao = PIND & 0b00100000; // Lê o estado do PD5
    if (botao == 0)                // Botão está pressionado ?
    {
      PORTD = PORTD | 0b10000000;    // PD7 -> HIGH
      _delay_ms(1000);               // Espera 5s
      PORTD = PORTD & ~(0b10000000); // PD7 -> LOW
    }
    int botao2 = PIND & 0b00010000; // Lê o estado do PD4
    if (botao2 == 0)                // Botão está pressionado ?
    {
      PORTD = PORTD | 0b01000000;    // PD6 -> HIGH
      _delay_ms(1000);               // Espera 5s
      PORTD = PORTD & ~(0b01000000); // PD6 -> LOW
    }
  }
  return 0;
}

#elif WHAT_CODE == QUESTION_1_C

int main(void)
{
  DDRD = DDRD | 0b11000000;      // Pino PD7 e PD6 definido como saída
  PORTD = PORTD | 0b00110000;    // Habilitar PULL-UP no PD5 e PD4
  PORTD = PORTD & ~(0b11000000); // Desliga a saída PD7 e PD6
  for (;;)                       // Super Loop
  {
    int botao = PIND & 0b00100000; // Lê o estado do PD5
    if (botao == 0)                // Botão está pressionado ?
    {
      PORTD = PORTD | 0b11000000;    // PD7 e PD6 -> HIGH
      _delay_ms(100);                // Espera 5s
      PORTD = PORTD & ~(0b11000000); // PD7 e PD6 -> LOW
    }

    int botao2 = PIND & 0b00010000; // Lê o estado do PD4
    if (botao2 == 0)                // Botão está pressionado ?
    {
      PORTD = PORTD | 0b11000000;    // PD7 e PD6 -> HIGH
      _delay_ms(1000);               // Espera 5s
      PORTD = PORTD & ~(0b11000000); // PD7 e PD6 -> LOW
    }
  }
}

#elif WHAT_CODE == QUESTION_2

int main(void)
{
  DDRD = DDRD | 0b11000000;      // Pino PD7 e PD6 definido como saída
  PORTD = PORTD | 0b00110000;    // Habilitar PULL-UP no PD5 e PD4
  PORTD = PORTD & ~(0b11000000); // Desliga a saída PD7 e PD6

  char estado = 0;
  while (1)
  {
    char btn1 = PIND & 0b00100000;
    char btn2 = PIND & 0b00010000;
    if (btn1 == 0)
    {
      estado = 1;
    }
    else if (btn2 == 0)
    {
      estado = 2;
    }
    else
    {
      estado = 0;
    }

    switch (estado)
    {
    case 0:
      PORTD = PORTD & ~(0b11000000); // PD7 e PD6 -> LOW
      break;

    case 1:
      // INVERTER PD7
      PORTD = PORTD ^ 0b10000000; // PD7 -> INVERTER

      // INVERTER PD6 em relação ao PD7
      if (PORTD & 0b10000000)
      {
        PORTD = PORTD & ~(0b01000000); // PD6 -> LOW
      }
      else
      {
        PORTD = PORTD | 0b01000000; // PD6 -> HIGH
      }
      break;

    case 2:
      // AMBOS ACESOS ou APAGADOS
      PORTD = PORTD ^ 0b11000000; // PD7 e PD6 -> INVERTER
      break;
    default:
      break;
    }

    _delay_ms(1000);
  }
}

#endif