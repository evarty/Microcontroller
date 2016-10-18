#include <avr/io.h> //allows more human readable stuff
//#include <avr/interrupt.h>  //allows interrupts
//#include <avr/power.h> //power reduction management
#define F_CPU 1000000
#include <util/delay.h>

#define PFETTopLeft 0
#define NFETBottomLeft 1
#define NFETBottomRight 2
#define PFETTopRight 3

int main(void){

  DDRD = 0xFF;
  DDRC = 0xFF;
  PORTD = (1 << PFETTopLeft) | (1 << PFETTopRight) | (0 << NFETBottomLeft) | (0 << NFETBottomRight);
  PORTC = (1 << PFETTopLeft) | (1 << PFETTopRight) | (0 << NFETBottomLeft) | (0 << NFETBottomRight);

  while(1){


  PORTD = (0 << PFETTopLeft) | (1 << PFETTopRight) | (0 << NFETBottomLeft) | (1 << NFETBottomRight);
  PORTC = (0 << PFETTopLeft) | (1 << PFETTopRight) | (0 << NFETBottomLeft) | (1 << NFETBottomRight);

  _delay_ms(250);

  PORTD = (1 << PFETTopLeft) | (1 << PFETTopRight) | (0 << NFETBottomLeft) | (0 << NFETBottomRight);
  PORTC = (1 << PFETTopLeft) | (1 << PFETTopRight) | (0 << NFETBottomLeft) | (0 << NFETBottomRight);

  _delay_ms(10);

  PORTD = (1 << PFETTopLeft) | (0 << PFETTopRight) | (1 << NFETBottomLeft) | (0 << NFETBottomRight);
  PORTC = (1 << PFETTopLeft) | (0 << PFETTopRight) | (1 << NFETBottomLeft) | (0 << NFETBottomRight);

  _delay_ms(250);


  PORTD = (1 << PFETTopLeft) | (1 << PFETTopRight) | (0 << NFETBottomLeft) | (0 << NFETBottomRight);
  PORTC = (1 << PFETTopLeft) | (1 << PFETTopRight) | (0 << NFETBottomLeft) | (0 << NFETBottomRight);

  _delay_ms(10);

  

  }
}
