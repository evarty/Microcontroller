#ifndef SPI_H

#define SPI_H

#include <avr/io.h>




void SPI_MasterInit(void);

uint8_t SPI_MasterTransmitByte(char cData);

void SPI_MasterTransmitArb(uint8_t *DataArray, uint8_t ArrayLength);

void SPI_MasterReceiveArb(uint8_t *DataArray, uint8_t ArrayLength);

void SPI_SlaveInit(void);

char SPI_SlaveRecieve(void);
#endif
