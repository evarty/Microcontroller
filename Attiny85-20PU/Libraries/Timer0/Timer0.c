//setup the clock
cli();                 //disable interrupts
TCCR1 |= (1<<CS12 | 1<<CS13 | 1<<CS10 | 1<CS11);     //divides clock by 4096. page 89 
OCR1A = 255;         //sets 488 as the max number of cycles to use in the timer register. page 1  36 or page 125
//TCCR1 |= 1<<CTC1;    //resets timer/counter 1 when it counts to value in OCR1C
//  TCCR1 |= 1<<WGM12;    //sets the clock mode. page 136
TIMSK |= 1<<OCIE1A;   //enables the interrupts for when the timer maxes out. page 92 
sei();                 //enables global interrupts


ISR(TIMER1_COMPA_vect)   //TIMER1_COMPA comes from page 65. the "vect" is required for compiler. The kind of interrupt must match TIMSK1 |= 1<<OCIE1A. The OCIE1A part
{
  //char llama = 0;
  PORTB ^= 1<<3;
}
