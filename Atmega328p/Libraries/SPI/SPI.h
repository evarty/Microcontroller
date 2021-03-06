#ifndef SPI_H

#define SPI_H

#include <avr/io.h>




void SPI_MasterInit(void);

uint8_t SPI_MasterTransmitByte(uint8_t Data);

void SPI_MasterTransmitArb(uint8_t *DataArray, uint8_t ArrayLength);

void SPI_MasterReceiveArb(uint16_t *DataArray, uint8_t ArrayLength);

void SPI_SlaveInit(void);

uint8_t SPI_SlaveRecieve(void);
#endif
