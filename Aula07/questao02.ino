// Declaração de bibliotecas
# include <Arduino.h>

// Declaração de constantes
# define VALVULA (1<<PD6) //led azul
# define CHEIO (1<<PD5) //led verde
# define ENCHENDO (1<<PD4) //led amarelo
# define ALARME (1<<PD7) //led vermelho
# define SENSOR_MIN (1<<PB0)
# define SENSOR_MAX (1<<PB1)
  
# define MIN_HIGH (PINB & SENSOR_MIN)
# define MAX_HIGH (PINB & SENSOR_MAX)

//main
int main(void){
  DDRD = VALVULA | CHEIO | ENCHENDO | ALARME; // Definido saidas
  PORTD &= ~VALVULA | CHEIO | ENCHENDO | ALARME; // VALVULA, CHEIO, ENCHENDO e ALARME vao iniciar em LOW

  // Loop infinito
  while(1){
    // se sensor_min tiver pressionado
    if(MIN_HIGH && !MAX_HIGH){
      PORTD &= ~ALARME;
      PORTD |= VALVULA;
      PORTD |= ENCHENDO;
      PORTD &= ~CHEIO;
      _delay_ms(1000);
    }
    // se sensor_max tiver pressionado
   if(MAX_HIGH && !MIN_HIGH){
      PORTD &= ~VALVULA;
      PORTD &= ~ENCHENDO;
      PORTD &= ~ALARME;
      PORTD |= CHEIO; 
      _delay_ms(1000);
    }
    // se sensor_min e sensor_max tiver não pressionado
    // se caso o líquido estiver no meio do tanque
    if(!MIN_HIGH && !MAX_HIGH){
      PORTD &= ~ALARME;
      PORTD |= VALVULA;
      PORTD |= ENCHENDO;
      PORTD &= ~CHEIO;
      _delay_ms(1000);
    }
    if(MIN_HIGH && MAX_HIGH){
      PORTD |= ALARME;
      PORTD &= ~VALVULA;
      PORTD &= ~CHEIO;
      PORTD &= ~ENCHENDO;
      _delay_ms(1000);
    }
  }
  return 0;
}