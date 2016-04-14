#include <avr/io.h>

#include "ShiftOut.h"

void ShiftOutByte(uint8_t CLKPin, volatile uint8_t *ClockPORT, uint8_t DataPin, volatile uint8_t *DataPORT, uint8_t LatchPin, volatile uint8_t *LatchPORT, uint8_t data){
  
  *LatchPORT &= ~(1 << LatchPin);
  
  for (int i = 0; i < 8; i++){

    *ClockPORT &= ~(1 << CLKPin);
    *DataPORT |= (((data >> i) & 0x01) << DataPin);
    *ClockPORT |= (1 << CLKPin); 
    *DataPORT &= ~(((data >> i) & 0x01) << DataPin);
    
  }

  *LatchPORT |= (1 << LatchPin); 

}

void ShiftOutMultiByte(uint8_t CLKPin, volatile uint8_t *ClockPORT, uint8_t DataPin, volatile uint8_t *DataPORT, uint8_t LatchPin, volatile uint8_t *LatchPORT, uint8_t *data, uint8_t NumDataByte){

  *LatchPORT &= ~(1 << LatchPin);
  
  for(int i = 0; i < (8 * NumDataByte); i++){
    
    *ClockPORT &= ~(1 << CLKPin);
    *DataPORT |= (((data[i] >> i ) & 0x01 ) << DataPin );
    *ClockPORT |= (1 << CLKPin);
    *DataPORT &= ~(((data[i] >> i ) & 0x01) << DataPin );
    
  }
  
  *LatchPORT |= (1 << LatchPin);

}

