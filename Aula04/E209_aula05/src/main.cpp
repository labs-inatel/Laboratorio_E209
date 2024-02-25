// Declaração de bibliotecas
#include <avr/io.h>

// Declaração de constantes

// Declaração de variáveis globais

// Declaração de funções

void main(void)
{
  // Direcionamento das Portas (DDRx) - Modificar só os pinos de saída
  DDRB = 0b00000001; // PB0 como saída (Bomba)
  DDRC = 0;          // Todos os pinos do portal são entradas
  DDRD = 0;          // Todos os pinos do portal são entradas

  // Habilita o circuito de PULL-UP nos botões NFs (PORTx)
  PORTC = 0b00000010; // PC1 com circuito de PULL-UP habilitado

  // Estabelecer a condição inicial das saídas
  PORTB = 0b00000000; // PB0 como 0 (Bomba desligada)

  // Loop infinito
  while (1)
  {
    // Leitura da Chave (PD3)
    short int chave;
    chave = PIND & 0b00001000; // Leitura do PD3

    if (chave == 0b00001000) // Se chave estiver em modo manual
    {
      // Leitura dos botões (PC0 e PC1)
      short int liga, desliga;
      desliga = PINC & 0b00000010; // Leitura do PC1
      liga = PINC & 0b00000001;    // Leitura do PC0

      if (desliga == 0b00000000) // Se botão desliga estiver pressionado
        PORTB = 0b00000000;      // PB0 como 0 (Bomba desligada)

      if (liga == 0b00000001) // Se botão liga estiver pressionado
        PORTB = 0b00000001;   // PB0 como 1 (Bomba ligada)
    }
    else
    { // Chave no modo automático
      // Leitura dos Sensores (Baixo e Alto)
      short int alto, baixo;
      baixo = PIND & 0b00010000; // Leitura do PD4
      alto = PIND & 0b00100000;  // Leitura do PD5

      if ((baixo == 0b00010000) && (alto == 0b00100000)) // Ambos acionados
        PORTB = 0b00000000;                              // PB0 como 0 (Bomba desligada)

      if ((baixo == 0) && (alto == 0)) // Ambos desacionados
        PORTB = 0b00000001;            // PB0 como 1 (Bomba ligada)

      
    }
  }
}