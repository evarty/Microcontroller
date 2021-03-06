#include <avr/pgmspace.h>
#include <stdio.h>
#include <avr/io.h>

#include "ShiftOut.h"
//int main(void){
//}


void ShiftOut(int SRCLK_Pin, int SER_Pin, int data){
  //PORTD &= ~(1 << RCLK_Pin); This is the Latch Pin                              // Set the register-clock pin low
  int i = 0;
  for (i = 0; i < (8); i++){     // Now we are entering the loop to shift out 8+ bits

    PORTB &= ~(1 << SRCLK_Pin);                     // Set the serial-clock pin low
    PORTB |= (((data&(0x01<<i))>>i) << SER_Pin );   // Go through each bit of data and output it
    PORTB |= (1 << SRCLK_Pin);                      // Set the serial-clock pin high
    PORTB &= ~(((data&(0x01<<i))>>i) << SER_Pin );  // Set the datapin low again    
  }

  //PORTD |= (1 << RCLK_Pin);                               // Set the register-clock pin high to update the output of the shift-register

}

