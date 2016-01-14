#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
#include <avr/interrupt.h>

#include "mirf.h"
#include "nRF24L01.h"
#include "spi.h"

int main(void){

  DDRD = 0x07;

  mirf_init();
  _delay_ms(100);

  sei();

  mirf_config();
  
  uint8_t buffer[1];

  while(1){

    buffer[0] = 0xAA;

    mirf_send(buffer, 1);
    //PORTD |= (1<<0);
    /*while(!mirf_data_ready()) {
      _delay_ms(100);
    }
    PORTD |= (1<<1);
    mirf_get_data(buffer);
    //PORTD |= (1<<2);
    */
  }


}
