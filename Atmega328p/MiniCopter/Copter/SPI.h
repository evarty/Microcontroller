#ifndef SPI_H

#define SPI_H






void SPI_MasterInit(void);

char SPI_MasterTransmitByte(char cData);

void SPI_SlaveInit(void);

char SPI_SlaveRecieve(void);
#endif
