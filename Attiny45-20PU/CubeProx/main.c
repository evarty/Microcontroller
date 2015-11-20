#include <avr/io.h>
#define F_CPU 1000000UL
#include <util/delay.h>

int main(void){

  _delay_ms(30000);
  DDRB = 0x04;
  PORTB = 0x00;

  while(1){

    if(PINB & 0x08){

      PORTB |= (1<<2);
      _delay_ms(50);
      PORTB &= ~(1<<2);
      _delay_ms(50);
    }
  }
}
