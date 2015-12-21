#ifndef nRF24_H
#define nRF24_H

void nRF24InitTransmit(volatile uint8_t *SSPort, uint8_t SSPin);
void nRF24InitReceive(volatile uint8_t *SSPort, uint8_t SSPin);
void WriteTXCharTransmit(char Data, volatile uint8_t *CEPort, uint8_t CEPin, volatile uint8_t *SSPort, uint8_t SSPin);//When calling, pass the port with an "&" in front of it. as in "&PORTB". its a pointer in the function, and you need up pass the address to the pointer. 

char ReadRXChar(volatile uint8_t *SSPort, uint8_t SSPin);

#endif