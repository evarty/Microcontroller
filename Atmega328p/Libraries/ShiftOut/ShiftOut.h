#ifndef SHIFTOUT_H
#define SHIFTOUT_H
//outputs a single byte on pins and ports demanded by the passed variables. 
void ShiftOutByte(uint8_t CLKPin, volatile uint8_t *ClockPORT, uint8_t DataPin, volatile uint8_t *DataPORT, uint8_t LatchPin, volatile uint8_t *LatchPORT, uint8_t data);
//outputs all entries in an array to shift register
void ShiftOutMultiByte(uint8_t CLKPin, volatile uint8_t *ClockPORT, uint8_t DataPin, volatile uint8_t *DataPORT, uint8_t LatchPin, volatile uint8_t *LatchPORT, uint8_t *data, uint8_t NumDataByte);
#endif
