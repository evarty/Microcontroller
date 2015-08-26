#include <avr/io.h> //allows more human readable stuff
#include <avr/interrupt.h>  //allows interrupts
#include <avr/power.h> //power reduction management

#include "ShiftOut.h"
#include "IIC.h"

#hmmm
#secondsry
#define LatchPin 2
#define DataPin 3
#define ClockPin 4

int main(void){
  DDRD = 0xff;
  DDRC = 0xff;
  uint8_t numbers[] = {252,96,218,242,102,182,62,224,254,230};
  uint8_t address = 0xD0; //1101000
  TWIInit();
  TWIStart();
  TWIWrite(address | (1<<0));
  TWIWrite(0x00);
  TWIWrite(0x00);
  TWIStop();

  while(1){  
//  IICTransmitMasterWrite(address, 0x01);
//  IICTransmitMasterWrite(address, 0xAA);
//  char Minutes = IICTransmitMasterRead(address);
//  char Hours = 0x0A;//IICTransmitMasterRead(address);

    static uint8_t MilTime = 0;
    if(ModePin & ModeMask){
      MilTime ^= 1;
      if(MilTime){
        TWIStart();
        TWIWrite(address | (0<<0));
        TWIWrite(0x02);
        TWIWrite(0x20);
        TWIStop();
      }else{
        TWIStart();
        TWIWrite(address | (0<<0);
        TWIWrite(0x02);
        TWIWrite(0x00);
        TWIStop();
      }

    TWIStart();
    TWIWrite(address | (0<<0));
    TWIWrite(0x01);
    TWIStart();
    TWIWrite(address | (1<<0));
    uint8_t Minutes = TWIReadACK();
    uint8_t Hours = TWIReadNACK();
    TWIStop();

    int MinutesOnes = (Minutes & 0x01) + (2*(Minutes & 0x02)) + (4*(Minutes & 0x04)) + (8*(Minutes & 0x08));
    int MinutesTens = (Minutes & 0x10) + (2*(Minutes & 0x20)) + (4*(Minutes & 0x40));
    int HourOnes = (Hours & 0x01) + (2*(Hours & 0x02)) + (4*(Hours & 0x04)) + (8*(Hours & 0x08));
    int HourTens = (Hours & 0x10)/* + (2*(Hours & 0x20))*/;

    PORTD &= ~(1 << LatchPin);
    ShiftOut(ClockPin,DataPin,numbers[HourOnes]);
    ShiftOut(ClockPin,DataPin,numbers[HourTens]);
    ShiftOut(ClockPin,DataPin,numbers[MinutesOnes]);
    ShiftOut(ClockPin,DataPin,numbers[MinutesTens]);
    PORTD |= (1 << LatchPin);
  }
}