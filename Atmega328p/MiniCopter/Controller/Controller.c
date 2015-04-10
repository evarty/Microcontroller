#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.>

#include "ADC.h"
#include "SPI.h"

#define VerticalPin 0
#define ForwardPin 1
#define SidePin 2


int main(void){

  ADCInitilize();
  SPI_MasterInit();

  char Vertical;
  char Forward;
  char Side;

  char VerticalCal = ADCRead(VerticalPin, 0);
  char ForwardCal = ADCRead(ForwardPin, 0);
  char SideCal = ADCRead(SidePin, 0);

  while(1){

    Vertical = ADCRead(VerticalPin, 0);
    Forward = ADCRead(ForwardPin, 0);
    Side = ADCRead(SidePin, 0); 











  }


}

