#include <avr/io.h>
#include "ADC.h"
#include "ShiftOut.h"

#define ResistorFixed 1.2
#define RefVoltage 5.
#define LatchPin 1
#define ClockPin 2
#define DataPin 3

int main(void){
  
  ADCInitilize();
  uint8_t numbers[] = {252,96,218,242,102,182,62,224,254,230};
  
  DDRD = 0xFF;
  PORTD |= 1<<LatchPin;

  for(;;){
    
    static uint16_t V_BatteryRead = ADCRead(1, 1);
    static uint16_t V_SetRead = ADCRead(1, 1);
    static uint16_t V_R_OutRead = ADCRead(1, 1);

    static uint16_t V_Battery = ((double)V_BatteryRead / 1024.) * RefVoltage;
    static uint16_t V_Set = ((double)V_SetRead / 1024.) * RefVoltage;
    static uint16_t V_R_Out = ((double)V_R_OutRead / 1024.) * RefVoltage;

    static uint16_t Power = (((double)V_Set / ResistorFixed) * (double)V_Battery) * 100;

    static uint8_t 

    PORTD &= ~(1<<LatchPin);
    ShiftOut(ClockPin, DataPin, numbers[         
















  }

}
