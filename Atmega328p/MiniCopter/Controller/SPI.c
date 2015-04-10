#include <avr/io.h>
#include "SPI.h"

#define DDR_SPI DDRB
#define SPI_MOSI_PIN 3
#define SPI_SCK_PIN 5
#define SPI_SS_PIN 2
#define SPI_MISO_PIN 4



void SPI_MasterInit(void){
  /* Set MOSI, SS, and SCK output, all others input */
  DDR_SPI = (1<<SPI_MOSI_PIN)|(1<<SPI_SCK_PIN);
  /* Enable SPI, Master, set clock rate fck/16 */
  SPCR = ((1<<SPE)|(1<<MSTR)|(1<<SPR0))&~(1<<DORD)&~(1<<CPHA)&~(1<<CPOL);
}

void SPI_MasterTransmitByte(char cData){
  /* Start transmission */
  SPDR = cData;
  /* Wait for transmission complete */
  while(!(SPSR & (1<<SPIF)));
}

void SPI_SlaveInit(void){
  /* Set MISO output, all others input */
  DDR_SPI = (1<<SPI_MISO_PIN);
  /* Enable SPI */
  SPCR = (1<<SPE);
}

char SPI_SlaveReceive(void){
  /* Wait for reception complete */
  while(!(SPSR & (1<<SPIF)));
  /* Return Data Register */
  return SPDR;
}
