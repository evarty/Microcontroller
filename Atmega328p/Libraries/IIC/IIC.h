#ifndef IIC_H
#define IIC_H

void TWIInit(void);//Initializes the TWI module 
void TWIStart(void);//Sends an IIC start signal. Can be used for repeated starts
void TWIStop(void);//Sends an IIC stop signal
void TWIWrite(uint8_t u8data);//sends single byte onto IIC bus
uint8_t TWIReadACK(void);//reads single byte with ACK signal
uint8_t TWIReadNACK(void);//reads single byte with NACK signal
uint8_t TWIGetStatus(void);//returns TWI status register



#endif
