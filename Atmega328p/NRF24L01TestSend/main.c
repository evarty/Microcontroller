#include <avr/io.h>
#define F_CPU 1000000
#include <util/delay.h>
#include <avr/interrupt.h>

#include "ADC.h"
//#include "SPI.h"
//#include "nRF24.h"

#include "mirf.h"
#include "nRF24L01.h"
#include "spi.h"

#define SSPin 2

int main(void){

  DDRB |= (1<<SSPin);  
  PORTB |= (1<<SSPin);
  DDRD |= (1<<0);
  PORTD |= (0<<0);
  //  ADCInitilize();
  //SPI_MasterInit();
  //nRF24InitTransmit(&PORTB, 2);
  //  char Vertical = 0;
  //  char Forward = 1;
  //  char Side = 2;

  spi_init();
  mirf_init();
  _delay_ms(50);

  sei();

  mirf_config();
  
  uint8_t buffer[1];

  while(1){

    buffer[0] = 0xAA;
    //  char VerticalReadValue = ADCRead(Vertical, 0);
    //  char ForwardReadValue = ADCRead(Forward, 0);
    //  char SideReadValue = ADCRead(Side, 0);

    //  float VerticalRatio = (float)VerticalReadValue / (256.0);
    //  float ForwardRatio = (float)ForwardReadValue / (256.0);
    //  float SideRatio = (float)SideReadValue / (256.0);
    //uint8_t VerticalReadValue = 0xAA;
    //WriteTXCharTransmit(VerticalReadValue, &PORTD, 0, &PORTB, 2);  

    //    PORTB &= ~(1<<DDB2);
    //    SPI_MasterTransmitByte(0xAA);
    //    PORTB |= (1<<DDB2);

    mirf_send(buffer, 1);
    while(!mirf_data_ready());
    mirf_get_data(buffer);

  }


}
