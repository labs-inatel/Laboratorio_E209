/*Questão 1-b)
int main(void) {
  DDRD = DDRD | 0b11000000; //PinoPD7ePD6definidocomosaída
  PORTD = PORTD | 0b00110000; //HabilitarPULL-UPnoPD5ePD4
  PORTD = PORTD & ~(0b11000000); //DesligaasaídaPD7
  
  for (;;) { // Super Loop
    int botao1 = PIND & 0b00100000; // Lê o estado do PD5 
    if (botao1 == 0) { // Botão está pressionado ?
      PORTD = PORTD | 0b10000000; // PD7 -> HIGH
      _delay_ms(1000); // Espera 100ms
      PORTD = PORTD & ~(0b10000000); // PD7 ->LOW
    }
    int botao2 = PIND & 0b00010000; // Lê o estado do PD4
    if (botao2 == 0) { // Botão está pressionado ?
      PORTD = PORTD | 0b01000000; // PD6 -> HIGH
      _delay_ms(1000); // Espera 1s
      PORTD = PORTD & ~(0b01000000); // PD6 ->LOW
    }
  }
}*/

/*Questão 1-c e 2)
int main(void) {
  DDRD = DDRD | 0b11000000; //PinoPD7ePD6definidocomosaída
  PORTD = PORTD | 0b00110000; //HabilitarPULL-UPnoPD5ePD4
  PORTD = PORTD & ~(0b11000000); //DesligaasaídaPD7
  
  for (;;) { // Super Loop
    int botao1 = PIND & 0b00100000; // Lê o estado do PD5 
    if (botao1 == 0) { // Botão está pressionado ?
      PORTD = PORTD | 0b11000000; // PD6 e PD6 -> HIGH
      _delay_ms(100); // Espera 100ms
      PORTD = PORTD & ~(0b11000000); // PD6 e PD7 ->LOW
    }
    int botao2 = PIND & 0b00010000; // Lê o estado do PD4
    if (botao2 == 0) { // Botão está pressionado ?
      PORTD = PORTD | 0b11000000; // PD6 e PD7 -> HIGH
      _delay_ms(1000); // Espera 1s
      PORTD = PORTD & ~(0b11000000); // PD6 e PD7 ->LOW
    }
  }
}*/



