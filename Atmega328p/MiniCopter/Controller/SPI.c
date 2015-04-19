#include <avr/io.h>
#include "SPI.h"

#define DDR_SPI DDRB
#define DD_MOSI 3
#define DD_SCK 5
#define DD_SS 2
#define SPI_MISO_PIN 4



void SPI_MasterInit(void){
  /* Set MOSI, SS, and SCK output, all others input */
  DDRB = (1<<DDB3)|(1<<DDB5)|(1<<DDB2);
  /* Enable SPI, Master, set clock rate fck/16 */
  SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);//&~(1<<DORD)&~(1<<CPHA)&~(1<<CPOL);
  PORTB |= (1<<DDB2);
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
