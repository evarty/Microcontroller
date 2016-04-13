#include <avr/io.h>

#include "ShiftOut.h"



void ShiftOutByte(uint8_t CLKPin, uint8_t *ClockPORT, uint8_t DataPin, uint8_t *DataPORT, uint8_t LatchPin, uint8_t *LatchPORT, uint8_t data){
  LatchPORT &= ~(1 << LatchPin);  // Set the Latch (register-clock) pin low
  
  for (int i = 0; i < 8; i++){     // Now we are entering the loop to shift out 8+ bits

    ClockPORT &= ~(1 << CLKPin);                     // Set the serial-clock pin low
    DataPORT |= (((data & (0x01 << i)) >> i) << DataPin );   // Go through each bit of data and output it
    ClockPORT |= (1 << CLKPin);                      // Set the serial-clock pin high
    DataPORT &= ~(((dat a& (0x01 << i)) >> i) << DataPin );  // Set the datapin low again    
  }

  LatchPORT |= (1 << LatchPin); 

}

void ShiftOutMultiByte(uint8_t CLKPin, uint8_t *ClockPORT, uint8_t DataPin, uint8_t *DataPORT, uint8_t LatchPin, uint8_t *LatchPORT, uint8_t *data, uint8_t NumDataByte){

  LatchPORT &= ~(1 << LatchPin);
  
  for(int i = 0; i < (8 * NumDataByte); i++){
    
    ClockPORT &= ~(1 << CLKPin);                     // Set the serial-clock pin low
    DataPORT |= (((data&(0x01<<i))>>i) << DataPin );   // Go through each bit of data and output it
    ClockPORT |= (1 << CLKPin);                      // Set the serial-clock pin high
    DataPORT &= ~(((data&(0x01<<i))>>i) << DataPin );  // Set the datapin low again    
    
  }
  
  LatchPORT |= (1 << LatchPin);

}

