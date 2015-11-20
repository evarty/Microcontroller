#include "SPI.h"
#include "nRF24.h"
#include <avr/io.h>



int main(void){

  SPI_MasterInit();
  nRF24InitReceive(&PORTB, 2);
  DDRD |= (1<<0)|(1<<7)|(1<<1);
  PORTD |= (1<<0)|(1<<7)|(0<<1);

  while(1){
  PORTD &= ~(1<<0);
  char ReadValue = ReadRXChar(&PORTB, 2);
  PORTD |= (1<<0);
//  PORTB &= ~(1<<2);
//  SPI_MasterTransmitByte(ReadValue);
//  PORTB |= (1<<2);
  if(ReadValue == 0xAA){
    PORTD |= (1<<1);
  }
  }  



}