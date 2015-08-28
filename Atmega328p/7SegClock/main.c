#include <avr/io.h> //allows more human readable stuff
#include <avr/interrupt.h>  //allows interrupts
#include <avr/power.h> //power reduction management

#include "ShiftOut.h"
#include "IIC.h"
#include "Timer0.h"

#define LatchPin 2
#define DataPin 3
#define ClockPin 4

#define MilMask 0x20
#define MilPin PINB
#define HourPin PINB
#define HourMask 0x20
#define MinutePin PINB
#define MinuteMask 0x20
//both preceding are arbitrary and currently placeholders

volatile bool MilTimeChange = false;
volatile bool HourAdd = false;
volatile bool MinuteAdd = false;

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
  
  Timer0SetupMode(0x00);
  Timer0SetupPrescale(0x60);
  Timer0SetupInterrupt(0x20);
  

  while(1){  
    
    static bool MilTime = false;

    if(MilTimeChange & !MilTime){//change to 24 hour clock
      TWIStart();
      TWIWrite(address | (0<<0));
      TWIWrite(0x02);
      TWIWrite(0x00);
      TWIStop();
      MilTime = true;
    }else if(MilTimeChange & MilTime){//change to 12 hour clock
      TWIStart();
      TWIWrite(address | (0<<0));
      TWIWrite(0x02);
      TWIWrite(0x40);
      TWIStop();
      MilTime = false;
    }
    

    TWIStart();
    TWIWrite(address | (0<<0));
    TWIWrite(0x01);
    TWIStart();
    TWIWrite(address | (1<<0));
    uint8_t Minutes = TWIReadACK();
    uint8_t Hours = TWIReadNACK();
    TWIStop();

    static uint8_t MinutesOnes = (Minutes & 0x01) + (2*(Minutes & 0x02)) + (4*(Minutes & 0x04)) + (8*(Minutes & 0x08));
    static uint8_t MinutesTens = (Minutes & 0x10) + (2*(Minutes & 0x20)) + (4*(Minutes & 0x40));
    static uint8_t HourOnes = (Hours & 0x01) + (2*(Hours & 0x02)) + (4*(Hours & 0x04)) + (8*(Hours & 0x08));
    if(MilTime){
      static uint8_t HourTens = (Hours & 0x10) + (2*(Hours & 0x20));
    }else {
      static uint8_t HourTens = (Hours & 0x10);
    }

    PORTD &= ~(1 << LatchPin);
    ShiftOut(ClockPin,DataPin,numbers[HourOnes]);
    ShiftOut(ClockPin,DataPin,numbers[HourTens]);
    ShiftOut(ClockPin,DataPin,numbers[MinutesOnes]);
    ShiftOut(ClockPin,DataPin,numbers[MinutesTens]);
    PORTD |= (1 << LatchPin);
    
    if(HourAdd){
      HourOnes += 1;
      if(MilTime){
        if((HourOnes == 4) & (HourTens == 2)){
          HourTens = 0;
          HourOnes = 0;
        }else if((HoursOnes == 10) & (HourTens < 2)){
          HourTens += 1;
          HourOnes = 0;
        }
      }else{
        if((HourOnes == 3) & (HourTens == 1)){
          HourTens = 0;
          HourOnes = 1;
        }else if((HoursOnes = 10) & (HoursTens < 1)){
          HourTens += 1;
          HourOnes = 0;
        }
      }
      
      if(MilTime){
        TWIStart();
        TWIWrite(address | (0 << 0));
        TWIWrite(0x02);
        TWIWrite(0x00 | (0<<6) | ((HourTens & 0x03)<<5) | ((HourOnes & 0x0F)<<3));
        TWIStop();
      }else {
        TWIStart();
        TWIWrite(address | (0<<0));
        TWIWrite(0x02);
        TWIWrite(0x00 | (1<<6) | ((HourTens & 0x03)<<5) | ((HourOnes & 0x0F)<<3));
        TWIStop();
      }
    }
    
    
    
    
    
  }
}

                 
ISR(TIMER0_OVF_vect){
  if(MilPin & MilMask){
    MilTimeChange = true;
  }else {
    MilTimeChange = false;
  }
  
  if(HourPin & HourMask){
    HourAdd = true;
  }else {
    HourAdd = false;
  }
  
  if(MinutePin & MinuteMask){
    MinuteAdd = true;
  }else{
    MinuteAdd = false;
  }
}