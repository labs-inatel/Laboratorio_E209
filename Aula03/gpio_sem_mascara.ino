/*///Questão 01:
int main() {
  DDRB = 0b00001110;
  
  while(true) {
    PORTB = 0b00001110;
    _delay_ms(1000);
  }
  return 0;
}

//Questão 02:
int main() {
  DDRB = 0b00001110;
  
  while(true) {
    PORTB = 0b0000000;
    _delay_ms(500);
    PORTB = 0b0000010;
    _delay_ms(500);
    PORTB = 0b0000100;
    _delay_ms(500);
    PORTB = 0b0000110;
    _delay_ms(500);
  }
  return 0;
}

//Questão 03:
int main() {
  DDRB = 0b00001110;
  
  while(true) {
    PORTB = 0b0000010;
    _delay_ms(250);
    PORTB = 0b0000100;
    _delay_ms(250);
  }
  return 0;
}/*/



