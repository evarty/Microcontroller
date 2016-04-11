//7 segment clock based on DS1307 for timekeeping
#define F_CPU 1000000
#include <avr/io.h> //allows more human readable stuff
#include <avr/interrupt.h>  //allows interrupts 
#include <util/delay.h>
#include <math.h>

#include "IIC.h"
#include "Timer0.h"

//define button pins
#define HourPort PINC
#define HourMask 0x01 //C0
#define MinutePort PINC
#define MinuteMask 0x02 //C1

//Function prototype
uint32_t ConvertToBar(uint8_t num);

//define variables that will be affected by ISR
volatile uint8_t HourButton = 0;
volatile uint8_t MinuteButton = 0;


int main(void){
  _delay_ms(50);
  //set up output pins
  DDRD = 0xFF;//DDRD |= (1 << 4) | (1 << 3) | (1 << 2);
  DDRC |= (1 << 5) | (1 << 4);
  DDRB = 0xFF;
  //define clock iic address
  uint8_t address = 0xD0; //1101000
  //uint8_t NumLED = 16;

  //init clock chip (ds1307)
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


  //set up timer0
  //  sei();
  //  Timer0SetupMode(0x00);
  //  Timer0SetupPrescale(0b01100000);
  //  Timer0SetupInterrupt(0x20);
  TCCR0A |= (0 << CS02);
  TCCR0B |= (1 << CS01) | (1 << CS00);
  TIMSK0 |= (1 << TOIE0);
  sei(); 

  while(1){  

    static uint8_t Minutes = 0, Hours = 0;
    //read current time from clock
    cli();
    TWIStart();
    TWIWrite(address | (0<<0));
    TWIWrite(0x01);
    TWIStart();
    TWIWrite(address | (1<<0));
    Minutes = TWIReadACK();
    Hours = TWIReadNACK();
    TWIStop();
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
    FracHour = (float)DecMinutes / (60.);
    FHours = (float)DecHours + FracHour;
    FracDay = FHours / (24.);
    //round to nearest LED
    LEDS = FracDay * (16.);
    LEDS = round(LEDS);
    IntLEDS = (uint8_t)LEDS;
    LIntLEDS = ConvertToBar(IntLEDS);
    //output to Bars
    PORTB = (LIntLEDS & 0xFF);
    PORTD = ((LIntLEDS >> 8) & 0xFF);

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
      TWIStart();
      TWIWrite(address | (0<<0));
      TWIWrite(0x02);
      TWIWrite(0x00 | (HoursTens << 4) | (HoursOnes << 0));
      TWIStop();
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


uint32_t ConvertToBar(uint8_t num){
  if(num > 32)
    return 0xFFFFFFFF;
 
  uint32_t hold = 0x00000000;
    
  for(int i = 0; i < num; i++){
    hold |= (1<<i);
  }
  
  return hold;
}
