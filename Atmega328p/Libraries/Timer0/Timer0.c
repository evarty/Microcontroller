#include <avr/io.h>
#include <avr/interrupt.h>


void Timer0SetupOutput(uint8_t output){
  TCCR0A |= ((output & 0x80) << COM0A0) | ((output & 0x40) << COM0A1) | ((output & 0x20) << COM0B0) | ((output & 0x10) << COM0B1);
}

void Timer0SetupMode(uint8_t mode){
  TCCR0B |= ((mode & 0x80) << WGM02);
  TCCR0A |= ((mode & 0x40) << WGM01) | ((mode & 0x20) << WGM00);
}

void Timer0SetupPrescale(uint8_t prescale){
  TCCR0B |= ((prescale & 0x80) << CS02) | ((prescale & 0x40) << CS01) | ((prescale & 0x20) << CS00);
}

void Timer0SetupInterrupt(uint8_t interrupt){
  TIMSK0 |= ((interrupt & 0x80) << OCIE0B) | ((interrupt & 0x40) << OCIE0A) | ((interrupt & 0x20) << TOIE0);
}

void Timer0SetCompareA(uint8_t comparea){
  OCR0A = comparea;
}

void Timer0SetCompareB(uint8_t compareb){
  OCR0B = compareb;
}

uint8_t Timer0ReadCounter(void){
  return TCNT0;
}
