#define F_CPU 8000000UL
#include <util/delay.h>

#include <avr/io.h>
#include <avr/interrupt.h>

int main(void)
{

  CLKPR = 0x80;
  CLKPR = 0x00;//Set main clock prescale to 1. 8Mhz

  TCCR0A |= (1<<WGM01) | (1<<WGM00);//Set into mode 7. PWM with OCR0A as TOP
  TCCR0B |= (1<<WGM02);//Part of previous line, dif register to complete config
  DDRB |= (1<<1);//Make PB1 a output
  TCCR0A |= (1<<COM0B1);//Noninverting PWM mode with output on OC0B (PB1).
  OCR0A = 0x10;//Set OCR0A (TOP) to 16. Makes generated frequency 8Mhz/16=500Khz

  //OCR0B = 5;
  //TCCR0B |= (1<<CS00);//Activate timer with no prescale

  //OCR0B = 50;
  uint8_t OutputState = 0;
  uint8_t ButtonPressed = 0;
  for(;;){

    if(PINB & 0x10){
      OCR0B = 10;
      TCCR0B |= (1<<CS00);
      _delay_ms(1000);
      TCCR0B &= ~(1<<CS00);
    }else{
      OCR0B = 5;
      TCCR0B |= (1<<CS00);
      _delay_ms(1000);
      TCCR0B &= ~(1<<CS00);
    }



  }
}

