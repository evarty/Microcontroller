#include <avr/io.h> //allows more human readable stuff
#include <avr/interrupt.h>  //allows interrupts
#include <avr/power.h> //power reduction management 

#include "ADC.h"
#include "SPI.h"

#define Kp 1
void PWM8BitOutput(uint8_t);
void PWM8BitSetup(void);
void ADCInit(void);
uint8_t ADCReadTest(uint8_t);

int main(void){

  ADCInit();

  //ADCInitilize();
  SPI_MasterInit();
  uint16_t *NewPosition = 0;
  //*NewPosition = 3000;
  uint16_t SetPoint;
  int16_t Error;
  uint8_t DutyCycle = 0;

  PWM8BitSetup();
  
  DDRD = 0xFF;
  DDRC = 0x00;

  while(1){

    SetPoint=ADCReadTest(0);//ADC pin 0 (PC0 Pin 23) and 8 bit resolution. 8 bit to try to avoid the ADC giving odd results.
    SPI_MasterReceiveArb(NewPosition,12);
    Error=(SetPoint*16) - *NewPosition;

    DutyCycle=(Error >> 4);

    if(Error > 0){
      PORTD &= ~(1 << PD5);
    }else{
      DutyCycle = 0xFF-DutyCycle;
      PORTD |= (1 << PD5);
    }

    //DutyCycle=(Kp*Error);
    //PWM8BitOutput(ReferencePosition);//DutyCycle);
    //PWM8BitOutput(ADCRead8Bit(0));
    PWM8BitOutput(DutyCycle);
  }
}

void PWM8BitOutput(uint8_t dutycycle){
  OCR0A=dutycycle;
}

void PWM8BitSetup(){
  TCCR0A |= 1 << COM0A1 | 0 << COM0A0 | 1 << WGM00 | 0 << WGM01;
  TCCR0B |= 1 << CS00;
}


void ADCInit(){

ADMUX |= (1 << REFS0) | (1 << ADLAR);
ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADEN);
}
uint8_t ADCReadTest(uint8_t Pin){

  ADMUX = (ADMUX & 0xF0) | (Pin & 0x0F);
  ADCSRA |= (1 << ADSC);
  while(ADCSRA & (1 << ADSC));
  return ADCH;

}