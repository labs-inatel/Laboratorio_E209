// Declaração de bibliotecas
# include <Arduino.h>

// Declaração de constantes
# define MOTOR_ABRE (1<<PD6) //led azul
# define MOTOR_FECHA (1<<PD5) //led verde
# define SENSOR (1<<PB2)
# define PARAR (1<<PB0)
# define COMANDO (1<<PB1)
  
# define COMANDO_HIGH (PINB & COMANDO)
# define PARAR_HIGH (PINB & PARAR)
# define LTSENSOR (PINB & SENSOR)

//main
int main(void){
  int i = 0; //contador
  DDRD = MOTOR_ABRE | MOTOR_FECHA; // Definido saidas
  PORTD &= ~MOTOR_ABRE| MOTOR_FECHA;// MOTOR_ABRE, MOTOR_FECHA e ALARME vao iniciar em LOW

  // Loop infinito
  while(1){
    // Caso se TOTALMENTE_FECHADO:
    if(!COMANDO_HIGH && !LTSENSOR){
      PORTD |= MOTOR_ABRE;
      PORTD &= ~MOTOR_FECHA;
      _delay_ms(1000);
    }
    // Caso se TOTALMENTE_ABERTO:
   if(!COMANDO_HIGH && LTSENSOR){
      PORTD &= ~MOTOR_ABRE;
      PORTD |= MOTOR_FECHA;
      _delay_ms(1000);
    }
    // Se caso botão parar for pressionado:
    if(!PARAR_HIGH){
      i++;
      if(i == 1){
        PORTD &= ~MOTOR_ABRE;
        PORTD &= ~MOTOR_FECHA;
        _delay_ms(1000);
      }
      if(i==2){
        // Caso se TOTALMENTE_FECHADO:
        if(LTSENSOR){
          PORTD |= MOTOR_ABRE;
          PORTD &= ~MOTOR_FECHA;
         _delay_ms(1000);
        }
       // Caso se TOTALMENTE_ABERTO:
        if(!LTSENSOR){
          PORTD &= ~MOTOR_ABRE;
          PORTD |= MOTOR_FECHA;
          _delay_ms(1000);
         }
        i=0;
       }
     }
  }
  return 0;
}