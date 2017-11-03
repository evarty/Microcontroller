#include <avr/io.h>
#include "SPI.h"

#define DDR_SPI DDRB
#define DD_MOSI 3
#define DD_SCK 5
#define SPI_MISO_PIN 4



void SPI_MasterInit(void){
  /* Set MOSI and SCK output, all others input */
  DDRB |= (1<<DDB3)|(1<<DDB5);
  /* Enable SPI, Master, set clock rate fck/16 */
  SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);//&~(1<<DORD)&~(1<<CPHA)&~(1<<CPOL);
}

uint8_t SPI_MasterTransmitByte(uint8_t Data){
  /* Start transmission */
  SPDR = Data;
  /* Wait for transmission complete */
  while(!(SPSR & (1<<SPIF)));

  return SPDR;
}

void SPI_MasterTransmitArb(uint8_t *DataArray, uint8_t ArrayLength){
  uint8_t i = 0;
  for(i = 0; i < ArrayLength; i++){
    SPI_MasterTransmitByte(DataArray[i]);
  }
}

void SPI_MasterReceiveArb(uint16_t *DataArray, uint8_t ArrayLength){
  uint8_t i = 0;
  for(i = 0; i < ArrayLength; i++){
    DataArray[i] = SPI_MasterTransmitByte(0x00);
  }
}

void SPI_SlaveInit(void){
  /* Set MISO output, all others input */
  DDR_SPI = (1<<SPI_MISO_PIN);
  /* Enable SPI */
  SPCR = (1<<SPE);
}

uint8_t SPI_SlaveReceive(void){
  /* Wait for reception complete */
  while(!(SPSR & (1<<SPIF)));
  /* Return Data Register */
  return SPDR;
}
