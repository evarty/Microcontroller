#include <avr/pgmspace.h>
#include <stdio.h>
#include <avr/io.h>

#include "ShiftOut.h"
//int main(void){
//}


void ShiftOut(int SRCLK_Pin, int SER_Pin, int data){
  //PORTD &= ~(1 << RCLK_Pin); This is the Latch Pin                              // Set the register-clock pin low
  for (int i = 0; i < 8; i++){     // Now we are entering the loop to shift out 8+ bits

    PORTD &= ~(1 << SRCLK_Pin);                     // Set the serial-clock pin low
    PORTD |= (((data&(0x01<<i))>>i) << SER_Pin );   // Go through each bit of data and output it
    PORTD |= (1 << SRCLK_Pin);                      // Set the serial-clock pin high
    PORTD &= ~(((data&(0x01<<i))>>i) << SER_Pin );  // Set the datapin low again    
  }

  //PORTD |= (1 << RCLK_Pin);                               // Set the register-clock pin high to update the output of the shift-register

}

void MultiByteShiftOut(uint8_t CLKPin, uint8_t *ClockPORT, uint8_t DataPin, uint8_t *DataPORT, uint8_t LatchPin, uint8_t *LatchPORT, uint8_t *data, uint8_t NumDataByte){

  LatchPORT &= ~(1 << LatchPin);
  
  for(int i = 0; i < (8 * NumDataByte); i++){
    
    ClockPORT &= ~(1 << CLKPin);                     // Set the serial-clock pin low
    DataPORT |= (((data&(0x01<<i))>>i) << DataPin );   // Go through each bit of data and output it
    ClockPORT |= (1 << CLKPin);                      // Set the serial-clock pin high
    DataPORT &= ~(((data&(0x01<<i))>>i) << DataPin );  // Set the datapin low again    
    
  }
  
  LatchPORT |= (1 << LatchPin);

}

