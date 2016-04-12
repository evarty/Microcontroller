#ifndef DS1307_H
#define DS1307_H

#define DS1307ADDRESS 0xD0

void DS1307Init(void);
void DS1307RegisterW(uint8_t reg, uint8_t data);
uint8_t DS1307RegisterR(uint8_t reg);
void DS1307RegisterWMult(uint8_t *reg, uint8_t RegLength, uint8_t *data, uint8_t dataLength);
void DS1307RegisterRMult(uint8_t FirstReg, uint8_t NumReg, uint8_t *data);

//Register nicknames


#endif
