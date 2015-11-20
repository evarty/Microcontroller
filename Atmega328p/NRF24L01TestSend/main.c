//#include <stdio.h>
#include <avr/io.h>
//#include <avr/pgmspace.h>
//#include <inttypes.h>

#include "ADC.h"
#include "SPI.h"
#include "nRF24.h"

#define SSPin 2

int main(void){

  DDRB |= (1<<SSPin);  
  PORTB |= (1<<SSPin);
  DDRD |= (1<<0);
  PORTD |= (0<<0);
  //  ADCInitilize();
  SPI_MasterInit();
  nRF24InitTransmit(&PORTB, 2);
  //  char Vertical = 0;
  //  char Forward = 1;
  //  char Side = 2;

  while(1){

    //  char VerticalReadValue = ADCRead(Vertical, 0);
    //  char ForwardReadValue = ADCRead(Forward, 0);
    //  char SideReadValue = ADCRead(Side, 0);

    //  float VerticalRatio = (float)VerticalReadValue / (256.0);
    //  float ForwardRatio = (float)ForwardReadValue / (256.0);
    //  float SideRatio = (float)SideReadValue / (256.0);
    uint8_t VerticalReadValue = 0xAA;
    WriteTXCharTransmit(VerticalReadValue, &PORTD, 0, &PORTB, 2);  

    //    PORTB &= ~(1<<DDB2);
    //    SPI_MasterTransmitByte(0xAA);
    //    PORTB |= (1<<DDB2);

  }


}

