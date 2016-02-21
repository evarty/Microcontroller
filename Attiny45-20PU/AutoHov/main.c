#include <avr/io.h>
#define F_CPU 8000000UL //Clock frequency
#include <util/delay.h>

#define RangeSensor 2
#define LiftControl OCR0A
#define LeftControl OCR1B
#define RightControl OCR0B

int main(void){

  DDRB = 0xFF;

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

    //LiftControl = 255;
/*    LeftControl = 64;
    RightControl = 64;

    PORTB |= (1<<RangeSensor);//Make the ultrasonic interface pin high
    _delay_us(5);//wait for adequate pulse to ultrasonic
    PORTB &= ~(1<<RangeSensor);//Bring ultrasonic pin low
    DDRB &= ~(1<<RangeSensor);//Set ultrasonic pin as input to detect return pulse
    _delay_us(200);//wait for time which corresponds to desired distance (200us ~3.5in)
    if(PINB & (1<<RangeSensor)){//if ultrasonic still outputting high, then any object is beyond 3.5in
      ; 
    }else{//if not still high, then pulse is ended, and craft is within 3.5in of obstacle
      LeftControl = 0;
      _delay_ms(500);
      LeftControl = 64;
    }
*/
    PORTB |= (1<<RangeSensor);
    _delay_us(5);
    PORTB &= ~(1<<RangeSensor);
    DDRB &= ~(1<RangeSensor);
    _delay_us(400);
    if(PINB & (1<<RangeSensor)){
      LiftControl = 1;
    }else{;}

  }
}
