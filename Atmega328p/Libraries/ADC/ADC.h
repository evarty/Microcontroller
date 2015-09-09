#ifndef ADC_H
#define ADC_H

void ADCInitilize(void);

uint16_t ADCRead(uint8_t ADCPin, uint8_t Resolution);

#endif
