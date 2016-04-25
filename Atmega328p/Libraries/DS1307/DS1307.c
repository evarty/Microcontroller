#include <avr/io.h>
#include "DS1307.h"
#include "IIC.h"


void DS1307Init(){//Initialize the DS1307; enable clock.

  TWIStart();
  TWIWrite(DS1307ADDRESS | (0 << 0));
  TWIWrite(0x00);
  TWIWrite(0x80);
  TWIStop();

}


void DS1307RegisterW(uint8_t reg, uint8_t data){//Write to a single register

    TWIStart();
    TWIWrite(DS1307ADDRESS | (0 << 0));
    TWIWrite(reg);
    TWIWrite(data);
    TWIStop();

}


uint8_t DS1307RegisterR(uint8_t reg){//Read from a single register

  uint8_t data = 0;
  
  TWIStart();
  TWIWrite(DS1307ADDRESS | (0 << 0));
  TWIWrite(reg);
  TWIStart();
  TWIWrite(DS1307ADDRESS | (1 << 0));
  data = TWIReadNACK();
  TWIStop();
  
  return data;

}

void DS1307RegisterWMult(uint8_t *reg, uint8_t NumReg, uint8_t *data, uint8_t dataLength){//Write to arbitrary number of registers

  for(uint8_t i = 0; i < NumReg; i++){
    TWIStart();
    TWIWrite(DS1307ADDRESS | (0 << 0));
    TWIWrite(reg[i]);
    TWIWrite(data[i]);
  }
  TWIStop();

}

void DS1307RegisterRMult(uint8_t FirstReg, uint8_t NumReg, uint8_t *data){//Read from arbitrary number of registers

  TWIStart();
  TWIWrite(DS1307ADDRESS | (0 << 0));
  TWIWrite(FirstReg);
  TWIStart();
  TWIWrite(DS1307ADDRESS | (1 << 0));
  for(uint8_t i = 0; i < (NumReg - 1); i++){
    data[i] = TWIReadACK();
  }
  data[NumReg] = TWIReadNACK();
}
