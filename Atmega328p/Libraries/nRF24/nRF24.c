#include "SPI.h"
#include "nRF24.h"
#define F_CPU 1000000UL

#include <util/delay.h>
#include <avr/io.h>



void nRF24InitTransmit(volatile uint8_t *SSPort, uint8_t SSPin){
  *SSPort &= ~(1<<SSPin);
  SPI_MasterTransmitByte(0x20);
  SPI_MasterTransmitByte(0x7E); //0b01111110
  *SSPort |= (1<<SSPin);
  *SSPort &= ~(1<<SSPin);
  SPI_MasterTransmitByte(0x23);
  SPI_MasterTransmitByte(0x03);
  *SSPort |= (1<<SSPin);
  *SSPort &= ~(1<<SSPin);
  SPI_MasterTransmitByte(0x24);
  SPI_MasterTransmitByte(0x0F);
  *SSPort |= (1<<SSPin);
  *SSPort &= ~(1<<SSPin);
  SPI_MasterTransmitByte(0x25);
  SPI_MasterTransmitByte(0x46);
  *SSPort |= (1<<SSPin);
  //*SSPort &= ~(1<<SSPin);
  //SPI_MasterTransmitByte(0x2B);
  //SPI_MasterTransmitByte(0x01);
  //*SSPort |= (1<<SSPin);
  
}

void nRF24InitReceive(volatile uint8_t *SSPort, uint8_t SSPin){
  *SSPort &= ~(1<<SSPin);
  SPI_MasterTransmitByte(0x20);
  SPI_MasterTransmitByte(0x7F); //0b01111111
  *SSPort |= (1<<SSPin);
  *SSPort &= ~(1<<SSPin);
  SPI_MasterTransmitByte(0x23);
  SPI_MasterTransmitByte(0x03);
  *SSPort |= (1<<SSPin);
  *SSPort &= ~(1<<SSPin);
  SPI_MasterTransmitByte(0x24);
  SPI_MasterTransmitByte(0x0F);
  *SSPort |= (1<<SSPin);
  *SSPort &= ~(1<<SSPin);
  SPI_MasterTransmitByte(0x25);
  SPI_MasterTransmitByte(0x46);
  *SSPort |= (1<<SSPin);
  //*SSPort &= ~(1<<SSPin);
  //SPI_MasterTransmitByte(0x2B);
  //SPI_MasterTransmitByte(0x01);
  //*SSPort |= (1<<SSPin);
  
}

void WriteTXCharTransmit(char Data, volatile uint8_t *CEPort, uint8_t CEPin, volatile uint8_t *SSPort, uint8_t SSPin){
  
  *SSPort &= ~(1<<SSPin);  
  SPI_MasterTransmitByte(0xA0);
  SPI_MasterTransmitByte(Data);
  *SSPort |= (1<<SSPin);

  *CEPort |= (1<<CEPin); 
  _delay_us(30);
  *CEPort &= ~(1<<CEPin);
}


char ReadRXChar(volatile uint8_t *SSPort, uint8_t SSPin){

  *SSPort &= ~(1<<SSPin);
  SPI_MasterTransmitByte(0x61);
  char hold = SPI_MasterTransmitByte(0xFF);
  *SSPort |= (1<<SSPin);
  //char hold = SPDR;
  return hold;

}
