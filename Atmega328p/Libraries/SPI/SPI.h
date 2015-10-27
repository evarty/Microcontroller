#ifndef SPI_H

#define SPI_H

#include <avr/io.h>




void SPI_MasterInit(void);

uint8_t SPI_MasterTransmitByte(char cData);

void SPI_SlaveInit(void);

char SPI_SlaveRecieve(void);
#endif
