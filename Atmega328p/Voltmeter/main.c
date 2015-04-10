#include <avr/io.h>
#include <stdio.h>
#include <avr/pgmspace.h>

#include "ShiftOut.h"

#define OneLatch 2
#define OneData 3
#define OneClock 4
void ADCInitilize(void)
{
  PORTC = 0x00;
  DDRC = 0x00;

  ADMUX |= 0<<REFS0 | 0<<REFS1;
  ADMUX |= 1<<ADLAR;
  ADCSRA |= 1<<ADEN | 1<<ADPS2 | 0<<ADPS1 | 0<<ADPS0 | 1<<ADSC;
  loop_until_bit_is_clear(ADCSRA,ADSC);
}

unsigned int ADCRead(void)
{
  unsigned int r;
  

  r = 0x10;
  ADMUX = r;
  ADCSRA |= 1<<ADSC;
  loop_until_bit_is_clear(ADCSRA,ADSC);
  char l = ADCL;
  char h = ADCH; 
  return ((int)h)<<8|l; 
}

int main(void)
{
  ADCInitilize();
  DDRD = 0xff;
  int numbers[] = {252,96,218,242,102,182,62,224,254,230};
  while(1)
  {
    unsigned int raw = ADCRead();
    float llama2; 
    llama2 = ((float)raw / 1024.) * 5.;
    int Tenths = (int)(10 * llama2) % 10;
    int Ones = (int)llama2 % 10;

    PORTD &= ~(1<<OneLatch);
    ShiftOut(OneClock,OneData,numbers[Tenths]);
    ShiftOut(OneClock,OneData,numbers[Ones]);
    PORTD |= 1<<OneLatch;
  }
}

