#ifndef Timer0_H
#define Timer0_H

void Timer0SetupOutput(uint8_t output);
void Timer0SetupMode(uint8_t mode);
void Timer0SetupPrescale(uint8_t prescale);
void Timer0SetupInterrupt(uint8_t interrupt);
void Timer0SetCompareA(uint8_t comparea);
void Timer0SetCompareB(uint8_t compareb);
uint8_t Timer0ReadCounter(void);

#endif