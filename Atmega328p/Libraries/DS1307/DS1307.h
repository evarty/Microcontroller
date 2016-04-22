#ifndef DS1307_H
#define DS1307_H

//Define clock chip address
#define DS1307ADDRESS 0xD0

void DS1307Init(void);//This initilizes the DS1307 and turns on the oscillator and puts it into military time mode
void DS1307RegisterW(uint8_t reg, uint8_t data);//This writes "data" to register "reg"
uint8_t DS1307RegisterR(uint8_t reg);//This reads the value of register "reg"
void DS1307RegisterWMult(uint8_t *reg, uint8_t NumReg, uint8_t *data, uint8_t dataLength);//This writes multiple registers
void DS1307RegisterRMult(uint8_t FirstReg, uint8_t NumReg, uint8_t *data);//This reads multiple registers

//Register nicknames
#define SecondsRegister 0x00
#define MinutesRegister 0x01
#define HoursRegister 0x02
#define DaysRegister 0x03
#define DateRegister 0x04
#define MonthRegister 0x05
#define YearRegister 0x06
#define ControlRegister 0x07
//0x08 through 0x3F are RAM

//Bit nicknames
#define ClockHaltBit 7//In SecondsRegister
#define MilTimeBit 6//In HoursRegister

#endif
