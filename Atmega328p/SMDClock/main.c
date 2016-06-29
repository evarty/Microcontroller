//7 segment clock based on DS1307 for timekeeping
#define F_CPU 1000000
#include <avr/io.h> //allows more human readable stuff
#include <avr/interrupt.h>  //allows interrupts 
#include <util/delay.h> //Includes delays

#include "IIC.h"//IIC library
#include "DS1307.h"//functions to interact with DS1307

//define button pins
#define HourPort PIND
#define HourMask 0x01 //D1
#define MinutePort PIND
#define MinuteMask 0x02 //B2


//define variables that will be affected by ISR
volatile uint8_t HourButton = 0;
volatile uint8_t MinuteButton = 0;


int main(void){
  _delay_ms(50);//brief delay to allow DS1307 to init
  //set up output pins
  DDRB = 0xFF;//set all Port B pins to be outputs
  DDRC = 0x0F;//set all Port C pins to be outputs
  //define 7 seg output values
  uint8_t numbers[] = {63,6,91,79,102,109,125,7,127,111};//numbers[i] outputted will show up as digit "i"


  //init clock chip (ds1307)
  TWIInit();
  DS1307Init();

  //set up timer0
  TCCR0A |= (0 << CS02);
  TCCR0B |= (1 << CS01) | (1 << CS00);
  TIMSK0 |= (1 << TOIE0);
  sei(); 

  while(1){  

    //define variables to hold current hours and minutes
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
    MinutesOnes = Minutes & 0x0F;//the ones value is the lower nibble
    MinutesTens = (Minutes & 0x70) >> 4;//the tens value is the lower 3 bits in the upper nibble
    HoursOnes = Hours & 0x0F;//the ones value is the lower nibble
    HoursTens = (Hours & 0x30) >> 4;//The tens values is the lower 2 bits in the upper nibble

    //define array for outputting
    uint8_t digit[] = {0, 0, 0, 0};
    digit[0] = numbers[HoursTens];
    digit[1] = numbers[HoursOnes];
    digit[2] = numbers[MinutesTens];
    digit[3] = numbers[MinutesOnes];

    //Output to seven segment
    for(uint8_t i = 0; i < 4; i++){
      PORTB = digit[i];
      PORTC |= (1 << i);
      _delay_ms(3);
      PORTC &= ~(1 << i);
    }

    //signal if adding an hour, deal with hour state
    if(HourButton && !HourState){//if the hour button has been pressed and the hour has not been dealt with yet, enter the conditional
      HourAdd = 1;//signal that the hour variable needs to be incremented
      HourState = 1;//set state variable
    }else if(!HourButton && HourState){//if the hour button has not been pressed and the state variable is still set, clear state bit.
      HourState = 0;
    }else {;}

    //signal if adding a minute, deal with minute state
    //same logic as the hours
    if(MinuteButton && !MinuteState){
      MinuteAdd = 1;
      MinuteState = 1;
    }else if(!MinuteButton && MinuteState){
      MinuteState = 0;
    }else {;}


    //add an hour, account for base 10 stuff
    if(HourAdd){
      HoursOnes+=1;
      if((HoursOnes == 4) && (HoursTens == 2)){
        HoursTens = 0;
        HoursOnes = 0;
      }else if((HoursOnes == 10) && (HoursTens < 2)){
        HoursTens += 1;
        HoursOnes = 0;
      }

      cli();
      DS1307RegisterW(0x02, 0x00 | (HoursTens << 4) | ((HoursOnes % 10) << 0));//write the new hours back to the DS1307
      sei();
      HourAdd = 0;
    }

    //add a minute, account for base 10 stuff
    if(MinuteAdd){
      MinutesOnes += 1;
      if((MinutesOnes == 10) && (MinutesTens == 5)){
        MinutesTens = 0;
        MinutesOnes = 0;
        HourAdd = 1;
      }else if((MinutesOnes == 10) && (MinutesTens < 6)){
        MinutesTens += 1;
        MinutesOnes = 0;
      }

      cli();
      DS1307RegisterW(0x01, 0x00 | (MinutesTens << 4) | (MinutesOnes << 0));//write the new minutes back to the DS1307
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
