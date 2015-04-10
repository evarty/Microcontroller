#include <avr/io.h>
#include <avr/interrupt.h>

//prepare clock
//prepare an interrupt every .125 seconds
//Setup IO for LEDs
//Turn off lit LED each interrupt
//turn on next LED each interrupt
//reverse direction every second

int main(void)
{


  //setup the clock
  cli();                 //disable interrupts
  TCCR1B |= 1<<CS11;     //divides clock by 8. page 137
  OCR1A = 15624;         //sets 15625 as the max number of cycles to use in the timer register. page 1  36 or page 125
  TCCR1B |= 1<<WGM12;    //sets the clock mode. page 136
  TIMSK1 |= 1<<OCIE1A;   //enables the interrupts for when the timer maxes out. page 139
  sei();                 //enables global interrupts

  DDRD |= 1<<0 | 1<<1 | 1<<2 | 1<<3 | 1<<4 | 1<<5 | 1<<6 | 1<<7;  //sets all D pins as output. page 76

  while(1){}
}

ISR(TIMER1_COMPA_vect)   //TIMER1_COMPA comes from page 65. the "vect" is required for compiler. The kind of interrupt must match TIMSK1 |= 1<<OCIE1A. The OCIE1A part
{
  char llama = 0;
  PORTD ^= 1<<llama;
}
