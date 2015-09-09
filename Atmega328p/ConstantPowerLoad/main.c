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
    
    static uint16_t V_BatteryRead;
    static uint16_t V_SetRead;
    static uint16_t V_R_OutRead;
    static uint16_t V_Battery;
    static uint16_t V_Set;
    static uint16_t V_R_Out;
    static uint16_t Power;
    static uint16_t PowerThousands;
    static uint16_t PowerHundreds;
    static uint16_t PowerTens;
    static uint16_t PowerOnes;

    V_BatteryRead = ADCRead(1, 1);
    V_SetRead = ADCRead(1, 1);
    V_R_OutRead = ADCRead(1, 1);

    V_Battery = ((double)V_BatteryRead / 1024.) * RefVoltage;
    V_Set = ((double)V_SetRead / 1024.) * RefVoltage;
    V_R_Out = ((double)V_R_OutRead / 1024.) * RefVoltage;

    Power = (((double)V_Set / ResistorFixed) * (double)V_Battery) * 1000;

    PowerThousands = (Power - (Power % 1000)) / 1000;
    PowerHundreds = (Power - (PowerThousands * 1000) - (Power % 100)) / 100;
    PowerTens = (Power - (PowerThousands * 1000) - (PowerHundreds * 100) - (Power % 10)) / 10;
    PowerOnes = (Power % 10);

    PORTD &= ~(1<<LatchPin);
    ShiftOut(ClockPin, DataPin, numbers[PowerOnes]);
    ShiftOut(ClockPin, DataPin, numbers[PowerTens]);
    ShiftOut(ClockPin, DataPin, numbers[PowerHundreds]);
    ShiftOut(ClockPin, DataPin, numbers[PowerThousands]);
    PORTD |= (1<<LatchPin);         
















  }

}
