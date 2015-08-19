#include <avr/io.h> //allows more human readable stuff
#include <avr/interrupt.h>  //allows interrupts
#include <avr/power.h> //power reduction management

#include "ShiftOut.h"
#include "IIC.h"

#define LatchPin 2
#define DataPin 3
#define ClockPin 4

int main(void){
  DDRD = 0xff;
  DDRC = 0xff;
  int numbers[] = {252,96,218,242,102,182,62,224,254,230};
  uint8_t address = 0xD0; //1101000
  TWIInit();


  while(1){  
//  IICTransmitMasterWrite(address, 0x01);
//  IICTransmitMasterWrite(address, 0xAA);
//  char Minutes = IICTransmitMasterRead(address);
//  char Hours = 0x0A;//IICTransmitMasterRead(address);

    TWIStart();
    TWIWrite(address | (0<<0));
    TWIWrite(0x01);
    TWIStop();

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
