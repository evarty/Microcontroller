//7 segment clock based on DS1307 for timekeeping
#define F_CPU 1000000
#include <avr/io.h> //allows more human readable stuff
#include <avr/interrupt.h>  //allows interrupts 
#include <util/delay.h> //Gives delay functions
#include <math.h> //Used in this case just for "round" because I didn't want to implement it myself.

#include "IIC.h" //IIC communication functions via TWI
#include "Timer0.h" //Setup and control of Timer0
#include "DS1307.h"//Clock chip functions

//define button pins
#define HourPort PINC
#define HourMask 0x01 //C0
#define MinutePort PINC
#define MinuteMask 0x02 //C1

//Use shift register for output
//#define USESHIFTREGISTER

#ifdef USESHIFTREGISTER
#include "ShiftOut.h"
#define ClockPin 0
#define ClockPORT DDRB
#define DataPin 1
#define DataPORT DDRB
#define LatchPin 2
#define LatchPORT DDRB
#endif

//Function prototype
uint32_t ConvertToBar(uint8_t num);

//define variables that will be affected by ISR
volatile uint8_t HourButton = 0;
volatile uint8_t MinuteButton = 0;


int main(void){
  _delay_ms(50);//the delay is to give the DS1307 time to initialize. 
  //set up output pins
  DDRD = 0xFF;//set all of port D to be output
  DDRB = 0xFF;//set all of port C to be output

  //init clock chip (ds1307)
  TWIInit();
  DS1307Init();

  //set up timer0 and enable interrupts
  TCCR0A |= (0 << CS02);
  TCCR0B |= (1 << CS01) | (1 << CS00);
  TIMSK0 |= (1 << TOIE0);
  sei(); 

  while(1){  

    //define time variables
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
    //define Bar variables
    static uint8_t DecMinutes = 0, DecHours = 0, IntLEDS = 0;
    static double FracHour = 0, FHours = 0, FracDay = 0, LEDS = 0;
    static uint32_t LIntLEDS = 0;

    //separate digits from read values
    MinutesOnes = Minutes & 0x0F;
    MinutesTens = (Minutes & 0x70) >> 4;
    HoursOnes = Hours & 0x0F;
    HoursTens = (Hours & 0x30) >> 4;

    //merge ones and tens digits and convert to base 10
    DecMinutes = MinutesOnes + 10*MinutesTens;
    DecHours = HoursOnes + 10*HoursTens;

    //Calculate fraction of Day that has occured
    FracHour = (double)DecMinutes / (60.);
    FHours = (double)DecHours + FracHour;
    FracDay = FHours / (24.);
    
    //round to nearest LED
    LEDS = FracDay * (16.);//exact number of LEDs needed
    LEDS = round(LEDS);//round to nearest integer
    IntLEDS = (uint8_t)LEDS & 0xFF;//pull the bottom byte into a smaller variable
    LIntLEDS = ConvertToBar(IntLEDS);//convert number of LEDs needed to a string of "1"s in a 32 bit int
    
    //output to Bars
    #ifndef USESHIFTREGISTER
    PORTB = (LIntLEDS & 0xFF);
    PORTD = ((LIntLEDS >> 8) & 0xFF);
    #endif
    #ifdef USESHIFTREGISTER
    ShiftOutByte(ClockPin, &ClockPORT, DataPin, &DataPORT, LatchPin, &LatchPORT, LIntLEDS & 0xFF);
    ShiftOutByte(ClockPin, &ClockPORT, DataPin, &DataPORT, LatchPin, &LatchPORT, (LIntLEDS >> 8) & 0xFF);

    #endif 

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

      //write new time back to DS1307
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

      //write new time back to DS1307
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

//converts integer to a string of "1"s at the low side of a 32 bit int. For example, "8" becomes "00000000000000000000000011111111"
//This imposes a 32 LED maximum, which can be easily expanded to 64 by using uint64_t.
//None of the 32 bit operations are atomic, but non of the variables are touched by the ISR, so it doesn't matter
uint32_t ConvertToBar(uint8_t num){
  if(num > 32)
    return 0xFFFFFFFF;
 
  uint32_t hold = 0x00000000;
    
  for(int i = 0; i < num; i++){
    hold |= (1<<i);
  }
  
  return hold;
}
