#include <math.h>
#include <avr/io.h>
#include "IIC.h"
#include "PCA9685.h"

#define PCA9685ADDRESS 0x80

void PCA9685Init(){
  
  TWIStart();
  TWIWrite(PCA9685ADDRESS | (0 << 0));
  TWIWrite(0x00);
  TWIWrite(0x10);
  TWIStart();
  TWIWrite(PCA9685ADDRESS | (0 << 0));
  TWIWrite(0x01);
  TWIWrite(0x0A);
  TWIStart();
  TWIWrite(PCA9685ADDRESS | (0 << 0));
  TWIWrite(0xFE);
  TWIWrite(0x79);
  TWIStart();
  TWIWrite(PCA9685ADDRESS | (0 << 0));
  TWIWrite(0x00);
  TWIWrite(0x00);
  TWIStop();

}

void PCA9685OutputNoPhase(uint8_t Driver, uint16_t DutyCycle){

  uint16_t LEDnOFF = (uint16_t)round(DutyCycle * 4096);
  uint8_t DutyBottom = LEDnOFF && 0x00FF;
  uint8_t DutyTop = (LEDnOFF >> 8) && 0x00FF;

  TWIStart();
  TWIWrite(PCA9685ADDRESS | (0 << 0));
  TWIWrite(0x08 + (4 * Driver));
  TWIWrite(DutyBottom);
  TWIWrite(DutyTop && 0x0F);
  TWIStop();

}
