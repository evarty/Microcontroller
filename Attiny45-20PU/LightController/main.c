#define F_CPU 8000000UL
#include <util/delay.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#define TurnOnMask 0x08
#define TurnOnPort PINB

int main(void)
{

  CLKPR = 0x80;
  CLKPR = 0x00;//Set main clock prescale to 1. 8Mhz

  TCCR0A |= (1<<WGM01) | (1<<WGM00);//Set into mode 7. PWM with OCR0A as TOP
  TCCR0B |= (1<<WGM02);//Part of previous line, different register to complete config
  DDRB |= (1<<1);//Make PB1 an output
  PORTB &= ~(1<<1);//Force PB1 Low
  TCCR0A |= (1<<COM0B1);//Noninverting PWM mode with output on OC0B (PB1).
  OCR0A = 0x10;//Set OCR0A (TOP) to 16. Makes generated frequency 8Mhz/16=500Khz

  //Init state variables
  uint8_t OnState = 0;
  uint8_t OffState = 0;
  uint8_t TurnOnEngaged = 0;
  uint8_t TurnOffEngaged = 0;

  for(;;){

    //Check whether the lights should be on or off. Because the two input pins are tied together, TurnOnEngaged and TurnOffEngaged will always be opposite. You don't actually need both variables.
    if(TurnOnMask & TurnOnPort){
      TurnOnEngaged = 1;
      TurnOffEngaged = 0;
    }else{
      TurnOnEngaged = 0;
      TurnOffEngaged = 1;
    }

    //If the lights should be on and they are not, turn them on
    if(TurnOnEngaged && !OnState){
      OnState = 1;//set state variable high to signify that the lights are on
      OCR0B = 11;//set compare point to 2/3 of OCR0A
      TCCR0B |= (1<<CS00);//Enable PWM clock and therefore output
      _delay_ms(100);
      TCCR0B &= ~(1<<CS00);//Disable PWM clock and therefore output
      _delay_ms(100);
      PORTB &= ~(1<<1);//Force PB1 low
      //If lights are on and signal is low, toggle onstate to off. 
    }else if(!TurnOnEngaged && OnState){
      OnState = 0;//set state variable
      PORTB &= ~(1<<1);//force PB1 Low
    }else{;}

    //Iff the lights are on and the turnoff signal is engaged, signal low, turn them off
    if(TurnOffEngaged && !OffState){
      OffState = 1;//set state variable
      OCR0B = 5;//Set compare point to 1/3 of OCR0A, 1/3 duty cycle
      TCCR0B |= (1<<CS00);//enable clock and therefore PWM output
      _delay_ms(100);
      TCCR0B &= ~(1<<CS00);//disable clock and therefor PWM output
      _delay_ms(100);
      PORTB &= ~(1<<1);//Force PB1 low
      //If lights are off and the signal pin is high, toggle offstate to false. 
    }else if(!TurnOffEngaged && OffState){
      OffState = 0;//set state variable
      PORTB &= ~(1<<1);//Force PB1 low
    }else{;}

  }
}

