#include <avr/io.h> //allows more human readable stuff
#include <avr/interrupt.h>  //allows interrupts
#include <avr/power.h> //power reduction management

#include "ShiftOut.h"
#include "IIC.h"
#include "Timer0.h"

#define LatchPin 2
#define DataPin 3
#define ClockPin 4

#define MilMask 0x08//D2
#define MilPin PINB
#define HourPort PINB
#define HourMask 0x02//D0
#define MinutePort PINB
#define MinuteMask 0x04//D1
//both preceding are arbitrary and currently placeholders

//volatile uint8_t MilTimeChange = 0;
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
  
  sei();
  //Timer0SetupMode(0x00);
  Timer0SetupPrescale(0x60);
  Timer0SetupInterrupt(0x10);
  

  while(1){  

    cli();
    TWIStart();
    TWIWrite(address | (0<<0));
    TWIWrite(0x01);
    TWIStart();
    TWIWrite(address | (1<<0));
    uint8_t Minutes = TWIReadACK();
    uint8_t Hours = TWIReadNACK();
    TWIStop();
    sei();
    
    static uint8_t HourState = 1, MinuteState = 1;
    static uint8_t MinutesOnes = 0, MinutesTens = 0, HoursOnes = 0, HoursTens = 0;

    MinutesOnes = Minutes & 0x0F;//(Minutes & 0x01) + (2*(Minutes & 0x02)) + (4*(Minutes & 0x04)) + (8*(Minutes & 0x08));
    MinutesTens = (Minutes & 0x70) >> 4;//(Minutes & 0x10) + (2*(Minutes & 0x20)) + (4*(Minutes & 0x40));
    HoursOnes = Hours & 0x0F;//(Hours & 0x01) + (2*(Hours & 0x02)) + (4*(Hours & 0x04)) + (8*(Hours & 0x08));
    HoursTens = (Hours & 0x30) >> 4;//(Hours & 0x10) + (2*(Hours & 0x20));

    PORTD &= ~(1 << LatchPin);
    ShiftOut(ClockPin,DataPin,numbers[HoursOnes]);
    ShiftOut(ClockPin,DataPin,numbers[HoursTens]);
    ShiftOut(ClockPin,DataPin,numbers[MinutesOnes]);
    ShiftOut(ClockPin,DataPin,numbers[MinutesTens]);
    PORTD |= (1 << LatchPin);


/*    
    if((HourPort & HourMask) && !HourState){
      HourAdd = 1;
      HourState = 1;
    }else if((!(HourPort & HourMask)) && HourState){
      HourState = 0;
    }else {;}

    if((MinutePort & MinuteMask) && !MinuteState){
      MinuteAdd = 1;
      MinuteState = 1;
    }else if((!(MinutePort & MinuteMask)) && MinuteState){
      MinuteState = 0;
    }else {;}
*/
    
    if(HourAdd){
      HoursOnes += 1;
      if((HoursOnes == 4) & (HoursTens == 2)){
        HoursTens = 0;
        HoursOnes = 0;
      }else if((HoursOnes == 10) & (HoursTens < 2)){
        HoursTens += 1;
        HoursOnes = 0;
      }
        
      cli();
      TWIStart();
      TWIWrite(address | (0<<0));
      TWIWrite(0x02);
      TWIWrite(0x00 | (HoursTens << 4) | (HoursOnes << 0));
      TWIStop();
      sei();
      HourAdd = 0;
    }

    if(MinuteAdd){
      MinutesOnes += 1;
      if((MinutesOnes == 0) & (MinutesTens == 6)){
        MinutesTens = 0;
        MinutesOnes = 0;
      }else if((MinutesOnes == 10) & (MinutesTens < 6)){
        MinutesTens += 1;
        MinutesOnes = 0;
      }
        
      cli();
      TWIStart();
      TWIWrite(address | (0<<0));
      TWIWrite(0x01);
      TWIWrite(0x00 | (MinutesTens << 4) | (MinutesOnes << 0));
      TWIStop();
      sei();

      MinuteAdd = 0;
    }

    
  }
}

                 
ISR(TIMER0_OVF_vect){

  if((HourPort & HourMask) && !HourState){
    HourAdd = 1;
    HourState = 1;
  }else if((!(HourPort & HourMask)) && HourState){
    HourState = 0;
  }else {;}
  
  if((MintuePort & MintueMask) && !MinuteState){
    MinuteAdd = 1;
    MinuteState = 1;
  }else if((!(MinutePort & MinuteMask)) && MinuteState){
    MinuteState = 1;
  }else {;}
  
  


}
