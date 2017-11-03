#include <avr/io.h> //allows more human readable stuff
//#include <avr/interrupt.h>  //allows interrupts
//#include <avr/power.h> //power reduction management
#define F_CPU 1000000
#include <util/delay.h>

#define H1RightNFET 0// H1 is on PortC
#define H1LeftNFET 1
#define H1RightPFET 2
#define H1LeftPFET 3
#define H2RightNFET 2// H2 is on PortD
#define H2LeftNFET 3
#define H2RightPFET 0
#define H2LeftPFET 1

int main(void){

  DDRD = 0xFF;
  DDRC = 0xFF;
  PORTD = (1 << H2RightPFET) | (1 << H2LeftPFET) | (0 << H2RightNFET) | (0 << H2LeftNFET);
  PORTC = (1 << H1RightPFET) | (1 << H1LeftPFET) | (0 << H1RightNFET) | (0 << H1LeftNFET);

  while(1){


  PORTD = (0 << H2RightPFET) | (1 << H2LeftPFET) | (0 << H2RightNFET) | (1 << H2LeftNFET);
  PORTC = (0 << H1RightPFET) | (1 << H1LeftPFET) | (0 << H1RightNFET) | (1 << H1LeftNFET);

  _delay_ms(1000);

  PORTD = (1 << H2RightPFET) | (1 << H2LeftPFET) | (0 << H2RightNFET) | (0 << H2LeftNFET);
  PORTC = (1 << H1RightPFET) | (1 << H1LeftPFET) | (0 << H1RightNFET) | (0 << H1LeftNFET);

  _delay_ms(10);

  PORTD = (1 << H2RightPFET) | (0 << H2LeftPFET) | (1 << H2RightNFET) | (0 << H2LeftNFET);
  PORTC = (1 << H1RightPFET) | (0 << H1LeftPFET) | (1 << H1RightNFET) | (0 << H1LeftNFET);

  _delay_ms(1000);


  PORTD = (1 << H2RightPFET) | (1 << H2LeftPFET) | (0 << H2RightNFET) | (0 << H2LeftNFET);
  PORTC = (1 << H1RightPFET) | (1 << H1LeftPFET) | (0 << H1RightNFET) | (0 << H1LeftNFET);

  _delay_ms(10);

  

  }
}
