#include <avr/io.h>

#include "ADC.h"

void ADCInitilize(void)
{
  PORTC = 0x00;
  DDRC = 0x00;

  ADMUX |= 0 << REFS0 | 0 << REFS1;
  ADMUX |= 1 << ADLAR;
  ADCSRA |= 1 << ADEN | 1 << ADPS2 | 1 << ADPS1 | 0 << ADPS0 | 1 << ADSC;
  loop_until_bit_is_clear(ADCSRA,ADSC);
}

uint16_t ADCRead(uint8_t ADCPin, uint8_t Resolution)
{
  uint8_t r;
  r = 0x10;
  r += ADCPin;
  ADMUX = r;
  ADCSRA |= 1 << ADSC;
  loop_until_bit_is_clear(ADCSRA,ADSC);
  uint8_t l = ADCL;
  uint8_t h = ADCH;

  if(Resolution){ 
    return 0x0000 | (h<<8) | (l<<0); 
  }else{
    return h;
  }
}

uint8_t ADCRead8Bit(uint8_t ADCPin)
{

  uint8_t r;
  r = 0x10;
  r += ADCPin;
  ADMUX = r;
  ADCSRA |= 1 << ADSC;
  loop_until_bit_is_clear(ADCSRA,ADSC);
  return ADCH;



}
