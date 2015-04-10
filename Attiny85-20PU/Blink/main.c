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
  TCCR1 |= (1<<CS12 | 1<<CS13 | 1<<CS10 | 1<CS11);     //divides clock by 4096. page 89 
  OCR1A = 20;         //sets 488 as the max number of cycles to use in the timer register. page 1  36 or page 125
  OCR1C = 122;          //set the number at which the timer/counter1 will equal OR1C for the reset in the next line
  TCCR1 |= 1<<CTC1;    //resets timer/counter 1 when it counts to value in OCR1C

  //The frequency is really determined by OCR1C and the CTC1 bit being set to 1, OCR1A simply generates interrupts regularly



//  TCCR1 |= 1<<WGM12;    //sets the clock mode. page 136
  TIMSK |= 1<<OCIE1A;   //enables the interrupts for when the timer maxes out. page 92 
  sei();                 //enables global interrupts

  DDRB |= 1<<0 | 1<<1 | 1<<2 | 1<<3 | 1<<4 | 1<<5 | 1<<6 | 1<<7;  //sets all D pins as output. page 76

  while(1){}
}

ISR(TIMER1_COMPA_vect)   //TIMER1_COMPA comes from page 65. the "vect" is required for compiler. The kind of interrupt must match TIMSK1 |= 1<<OCIE1A. The OCIE1A part
{
  //char llama = 0;
  PORTB ^= 1<<3;
}
