#include <avr/io.h> //allows more human readable stuff
#include <avr/interrupt.h>  //allows interrupts
#include <avr/power.h> //power reduction management

int main(void){

  DDRB = 0xFF;
  DDRC = 0x0F;

  while(1){

    PORTB = 0xFF;
    PORTC = 0xFF;

  }
}
