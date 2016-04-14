//7 segment clock based on DS1307 for timekeeping
#define F_CPU 1000000
#include <avr/io.h> //allows more human readable stuff
#include <avr/interrupt.h>  //allows interrupts 
#include <util/delay.h>

#include "ShiftOut.h"
#include "IIC.h"
#include "Timer0.h"
#include "DS1307.h"


//define pins for 7 seg output 
#define LatchPin 3
#define DataPin 4
#define ClockPin 2


//define button pins
#define HourPort PINB
#define HourMask 0x02 //B1
#define MinutePort PINB
#define MinuteMask 0x04 //B2


//define variables that will be affected by ISR
volatile uint8_t HourButton = 0;
volatile uint8_t MinuteButton = 0;


int main(void){
  _delay_ms(50);
  //set up output pins
  DDRD = 0xFF;//DDRD |= (1 << 4) | (1 << 3) | (1 << 2);
  //define 7 seg output values
  uint8_t numbers[] = {252,96,218,242,102,182,62,224,254,230};
  //define clock iic address
  uint8_t address = 0xD0; //1101000


  //init clock chip (ds1307)
  TWIInit();
  DS1307Init();

  //set up timer0
  TCCR0A |= (0 << CS02);
  TCCR0B |= (1 << CS01) | (1 << CS00);
  TIMSK0 |= (1 << TOIE0);
  sei(); 

  while(1){  

    static uint8_t Minutes = 0, Hours = 0;
    //read current time from clock
    cli();
    Minutes = DS1307RegisterR(0x01);
    Hours = DS1307RegisterR(0x02);
    sei();

    //define state variables
    static uint8_t HourAdd = 0, MinuteAdd = 0, HourState = 0, MinuteState = 0;
    //define working variables
    static uint8_t MinutesOnes = 0, MinutesTens = 0, HoursOnes = 0, HoursTens = 0;

    //separate digits from read values
    MinutesOnes = Minutes & 0x0F;
    MinutesTens = (Minutes & 0x70) >> 4;
    HoursOnes = Hours & 0x0F;
    HoursTens = (Hours & 0x30) >> 4;

    //output to 7 segs
    ShiftOutByte(ClockPin, &PORTD, DataPin, &PORTD, LatchPin, &PORTD, numbers[MinutesOnes]);
    ShiftOutByte(ClockPin, &PORTD, DataPin, &PORTD, LatchPin, &PORTD, numbers[MinutesTens]);
    ShiftOutByte(ClockPin, &PORTD, DataPin, &PORTD, LatchPin, &PORTD, numbers[HoursOnes]);
    ShiftOutByte(ClockPin, &PORTD, DataPin, &PORTD, LatchPin, &PORTD, numbers[HoursTens]);


    //signal if adding an hour, deal with hour state
    if(HourButton && !HourState){
      HourAdd = 1;
      HourState = 1;
    }else if(!HourButton && HourState){
      HourState = 0;
    }else {;}

    //signal if adding a minute, deal with minute state
    if(MinuteButton && !MinuteState){
      MinuteAdd = 1;
      MinuteState = 1;
    }else if(!MinuteButton && MinuteState){
      MinuteState = 0;
    }else {;}


    //add an hour, account for base 10 stuff
    if(HourAdd){
      HoursOnes += 1;
      if((HoursOnes == 4) && (HoursTens == 2)){
        HoursTens = 0;
        HoursOnes = 0;
      }else if((HoursOnes == 10) && (HoursTens < 2)){
        HoursTens += 1;
        HoursOnes = 0;
      }

      cli();
      DS1307RegisterW(0x02, 0x00 | (HoursTens << 4) | (HoursOnes << 0));
      sei();
      HourAdd = 0;
    }

    //add a minute, account for base 10 stuff
    if(MinuteAdd){
      MinutesOnes += 1;
      if((MinutesOnes == 10) && (MinutesTens == 5)){
        MinutesTens = 0;
        MinutesOnes = 0;
      }else if((MinutesOnes == 10) && (MinutesTens < 6)){
        MinutesTens += 1;
        MinutesOnes = 0;
      }

      cli();
      DS1307RegisterW(0x01, 0x00 | (MinutesTens << 4) | (MinutesOnes << 0));
      sei();

      MinuteAdd = 0;
    }


  }
}


ISR(TIMER0_OVF_vect){//ISR takes ~14 us, occurs every ~15ms

  //check if button pressed and indicate either way
  if(HourPort & HourMask){
    HourButton = 1;
  }else{
    HourButton = 0;
  }

  if(MinutePort & MinuteMask){
    MinuteButton = 1;
  }else{
    MinuteButton = 0;
  } 

}
