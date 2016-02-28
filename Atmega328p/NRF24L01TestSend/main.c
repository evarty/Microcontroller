#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
#include <avr/interrupt.h>

#include "mirf.h"
#include "nRF24L01.h"
#include "spi.h"

int main(void){

  DDRD |= (1<<0);

  mirf_init();
  _delay_ms(100);

  sei();

  mirf_config();
  
  uint8_t buffer[3];
//  uint8_t recbuffer[1];
//  recbuffer[0] = 0x00;

  while(1){

    buffer[0] = 0xAA;
    buffer[1] = 0xFF;
    buffer[2] = 0xFA;

    mirf_send(buffer, 3);

    //_delay_ms(100);

    /*if(mirf_data_ready()){ 
    mirf_get_data(recbuffer);
    }

    if (recbuffer[0] == 0xFA){
      PORTD |= (1<<0);
    }
   */ 







  }
}
