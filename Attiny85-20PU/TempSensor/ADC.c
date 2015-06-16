#include <avr/pgmspace.h>
#include "ADC.h"

void ADCInitChar(void){

  ADMUX |= (1<<REFS2)|(1<<REFS1)|(1<<ADLAR);
  ADCSRA |= (1<<ADEN)|(1<<ADPS2)|(1<<ADSC);
  loop_until_bit_is_clear(ADCSRA,ADSC);

}

char ADCReadChar(char Pin){
  if(Pin == 0){
  }else
  if(Pin == 1){
    ADMUX |= (1<<MUX0);
  }else
  if(Pin == 2){
    ADMUX |= (1<<MUX1);
  }else
  if(Pin == 3){
    ADMUX |= (1<<MUX1)|(1<<MUX0);
  }else
  if(Pin == 4){
    ADMUX |= (1<<MUX0)|(1<<MUX1)|(1<<MUX2)|(1<<MUX3);
  }
  
  ADCSRA |= 1<<ADSC;
  loop_until_bit_is_clear(ADCSRA,ADSC);
  
  return ADCH;
}
