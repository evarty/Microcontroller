#include <avr/io.h> //allows more human readable stuff
#include <avr/interrupt.h>  //allows interrupts
#include <math.h> //math shit functions and constants
#include <stdint.h> //standard integers
#include <stdlib.h> //general utilities
#include <avr/power.h> //power reduction management

#include "ShiftOut.h"
#include "IIC.h"

#define LatchPin 2
#define DataPin 3
#define ClockPin 4

void main(){
DDRD = 0xff;
int numbers[] = {252,96,218,242,102,182,62,224,254,230};
char address = 0; //1101000
address |= (1<<7) | (1<<6) | (1<<4);












PORTD &= ~(1 << LatchPin);
ShiftOut(ClockPin,DataPin,numbers[HourOnes]);
ShiftOut(ClockPin,DataPin,numbers[HourTens]);
ShfitOut(ClockPin,DataPin,numbers[MinutesOnes]);
ShiftOut(ClockPin,DataPin,numbers[MinutesTens]);
PORTD |= (1 << LatchPin);









}
