//#include "SPI.h"
//#include "nRF24.h"
#include <avr/io.h>

#define F_CPU 8000000
#include <util/delay.h>
#include <avr/interrupt.h>

#include "mirf.h"
#include "nRF24L01.h"
#include "spi.h"

int main(void){

  //SPI_MasterInit();
  //nRF24InitReceive(&PORTB, 2);
  //DDRD |= (1<<0)|(1<<7)|(1<<1);
  //PORTD |= (1<<0)|(1<<7)|(0<<1);
  //DDRB |= (1<<2);
  DDRD |= (1<<1);

  mirf_init();
  _delay_ms(50);

  sei();

  mirf_config();

  uint8_t buffer[1];

  while(1){
    //PORTD &= ~(1<<0);
    //char ReadValue = ReadRXChar(&PORTB, 2);
    //PORTD |= (1<<0);
    //  PORTB &= ~(1<<2);
    //  SPI_MasterTransmitByte(ReadValue);
    //  PORTB |= (1<<2);

      mirf_get_data(buffer);

    if(buffer[0] == 0xAA){
      PORTD |= (1<<1);
    }
  }  



}
