#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "ShiftOut.h"
#include "ADC.h"

int main(void){

  ADCInitChar();
  DDRB |= (1<<2)|(1<<0)|(1<<1);
  char Temp;
  float Hold;
  float Read;

  while(1){

    Read = ADCReadChar(2);
    Hold = (((Read / 100.) - .75 ) * 100.) + 25;
    Temp = (char)(Hold);
    PORTB &= ~(1<<1);
    ShiftOut(2, 0, Temp);
    PORTB |= (1<<1);
  }

}
