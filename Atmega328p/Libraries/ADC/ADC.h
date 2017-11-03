#ifndef ADC_H
#define ADC_H

void ADCInitilize(void);

uint16_t ADCRead(uint8_t ADCPin, uint8_t Resolution);

uint8_t ADCRead8Bit(uint8_t ADCPin);

#endif
