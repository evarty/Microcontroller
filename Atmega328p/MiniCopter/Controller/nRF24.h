#ifndef nRF24_H
#define nRF24_H
#include <inttypes.h>

void nRF24Init(volatile uint8_t *SSPort, uint8_t SSpin);

void WriteTXCharTransmit(char Data, volatile uint8_t *CEPort, uint8_t CEPin, volatile uint8_t *SSPort, uint8_t SSPin);//When calling, pass the port with an "&" in front of it. as in "&PORTB"

char ReadRXChar(char address, volatile uint8_t *SSPort, uint8_t SSPin);

#endif
