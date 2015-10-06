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

volatile uint8_t MilTimeChange = 0;
volatile uint8_t HourAdd = 0;
volatile uint8_t MinuteAdd = 0;

int main(void){
  DDRD = 0xff;
  DDRC = 0xff;
  uint8_t numbers[] = {252,96,218,242,102,182,62,224,254,230};
  uint8_t address = 0xD0; //1101000
  TWIInit();
  TWIStart();
  TWIWrite(address | (0<<0));
  TWIWrite(0x00);
  TWIWrite(0x00);
  TWIStart();
  TWIWrite(address | (0<<0));
  TWIWrite(0x07);
  TWIWrite(0x03);
  TWIStop();
  
  Timer0SetupMode(0x00);
  Timer0SetupPrescale(0x60);
  Timer0SetupInterrupt(0x20);
  

  while(1){  


    TWIStart();
    TWIWrite(address | (0<<0));
    TWIWrite(0x01);
    TWIStart();
    TWIWrite(address | (1<<0));
    uint8_t Minutes = TWIReadACK();
    uint8_t Hours = TWIReadNACK();
    TWIStop();
    
    static uint8_t MilTime = 0;//Defaults to 12 hour clock
    static uint8_t MinutesOnes, MinutesTens, HoursOnes, HoursTens;

    MinutesOnes = Minutes & 0x0F;//(Minutes & 0x01) + (2*(Minutes & 0x02)) + (4*(Minutes & 0x04)) + (8*(Minutes & 0x08));
    MinutesTens = Minutes & 0x70;//(Minutes & 0x10) + (2*(Minutes & 0x20)) + (4*(Minutes & 0x40));
    HoursOnes = Hours & 0x0F;//(Hours & 0x01) + (2*(Hours & 0x02)) + (4*(Hours & 0x04)) + (8*(Hours & 0x08));
    if(MilTime){
      HoursTens = Hours & 0x30;//(Hours & 0x10) + (2*(Hours & 0x20));
    }else {
      HoursTens = (Hours & 0x10);
    }

    PORTD &= ~(1 << LatchPin);
    ShiftOut(ClockPin,DataPin,numbers[HoursOnes]);
    ShiftOut(ClockPin,DataPin,numbers[HoursTens]);
    ShiftOut(ClockPin,DataPin,numbers[MinutesOnes]);
    ShiftOut(ClockPin,DataPin,numbers[MinutesTens]);
    PORTD |= (1 << LatchPin);
    
    
    
    
    if(MilTimeChange & !MilTime){//change to 24 hour clock
      TWIStart();
      TWIWrite(address | 0<<0);//Write to register
      TWIWrite(0x02);//First write sets pointer to hours register
      TWIWrite(0x00);//Write whole hours register to 0. Changing from 12 to 24 resets hours and minutes anway
      TWIStart();//repeated start
      TWIWrite(address | 0<<0);
      TWIWrite(0x01);
      TWIWrite(Minutes);
      TWIWrite(Hours & 0x3F);
      TWIStop();
      MilTime = 1;
    }else if(MilTimeChange & MilTime){//Change to 12 hour clock
      TWIStart();
      TWIWrite(address | 0<<0);
      TWIWrite(0x02);
      TWIWrite(0x40);//Writes bit 6 to 1 which enables 12 hour mode
      TWIStart();
      TWIWrite(address | 0<<0);
      TWIWrite(0x01);
      TWIWrite(Minutes);
      TWIWrite(Hours & 0x7F);
      TWIStop();
      MilTime = 0;
    }
    
    
/*    
    if(HourAdd){
      HoursOnes += 1;
      if(MilTime){
        if((HoursOnes == 4) & (HoursTens == 2)){
          HoursTens = 0;
          HoursOnes = 0;
        }else if((HoursOnes == 10) & (HoursTens < 2)){
          HoursTens += 1;
          HoursOnes = 0;
        }
      }else{
        if((HoursOnes == 3) & (HoursTens == 1)){
          HoursTens = 0;
          HoursOnes = 1;
        }else if((HoursOnes = 10) & (HoursTens < 1)){
          HoursTens += 1;
          HoursOnes = 0;
        }
      }
      
    
*/    
    
    
  }
}

                 
ISR(TIMER0_OVF_vect){
  if(MilPin & MilMask){
    MilTimeChange = 1;
  }else {
    MilTimeChange = 0;
  }
  
  if(HourPin & HourMask){
    HourAdd = 1;
  }else {
    HourAdd = 0;
  }
  
  if(MinutePin & MinuteMask){
    MinuteAdd = 1;
  }else{
    MinuteAdd = 0;
  }
}
