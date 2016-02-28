#include <avr/io.h>
#define F_CPU 8000000UL //Clock frequency
#include <util/delay.h>

#define LiftControl OCR0A
#define LeftControl OCR1B
#define RightControl OCR0B
#define LeftGate0 3
#define LeftGate1 2

int main(void){

  DDRB = 0xFF;
  PORTB = 0x00;

  CLKPR = 0x80;
  CLKPR = 0x00;//Set main clock prescale to 1. clock:8MHz

  TCCR0A |= (1<<WGM01) | (1<<WGM00);//Set Timer0 into mode 3. Fast PWM with MAX (0xFF) as TOP
  TCCR0A |= (1<<COM0A1) | (1<<COM0B1);//Set Timer0 for non-inverting PWM mode
  TCCR0B |= (1<<CS00);//Set for no prescaler

  TCCR1 |= (1<<CS10) | (1<<COM1A0);//Set for no prescaler COM1A0 is set to account for hardware bug
  GTCCR |= (1<<PWM1B) | (1<<COM1B0);//Enable OC1B pwm. set toggle on match
  OCR1C = 0xFF;//Set max count for Timer1 PWM


  //  uint8_t LiftPWM = 0;//Will be pushed into OCR0A to control speed of lifting motor
  //  uint8_t LeftPWM = 0;//Will be pushed into OCR1B to control speed of left motor
  //  uint8_t RightPWM = 0;//Will be pushed into OCR0B to control speed of right motor
    //LiftControl = 128;
  while(1){

    LiftControl = 0;
    LeftControl = 1;
    RightControl = 0;

    PORTB &= ~(1<<LeftGate1);
    PORTB |= (1<<LeftGate0);

    _delay_ms(3000);

    PORTB &= ~(1<<LeftGate0);
    PORTB |= (1<<LeftGate1);

    _delay_ms(1000);

    PORTB &= ~(1<<LeftGate1);
    PORTB |= (1<<LeftGate0);

  }
}
