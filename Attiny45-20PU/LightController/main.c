#define F_CPU 8000000UL
#include <util/delay.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#define TurnOnMask 0x02
#define TurnOnPort PINB
#define TurnOffMask 0x04
#define TurnOffPort PINB

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
  uint8_t OnState = 0;
  uint8_t OffState = 0;
  uint8_t TurnOnEngaged = 0;
  uint8_t TurnOffEngaged = 0;

  for(;;){

    if(TurnOnMask & TurnOnPort){
      TurnOnEngaged = 1;
    }else{
      TurnOnEngaged = 0;
    }

    if(TurnOffMask & TurnOffPort){
      TurnOffEngaged = 1;
    }else{
      TurnOffEngaged = 0;
    }

    if(TurnOnEngaged && !OnState){
      OnState = 1;
      OCR0B = 10;
      TCCR0B |= (1<<CS00);
    }else if(!TurnOnEngaged && OnState){
      OnState = 0;
    }else{;}

    if(TurnOffEngaged && !OffState){
      OffState = 1;
      OCR0B = 5;
      TCCR0B |= (1<<CS00);
    }else if(!TurnOffEngaged && OffState){
      OffState = 0;
    }else{;}


  }
}

