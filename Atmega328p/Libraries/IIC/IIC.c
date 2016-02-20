#include <avr/io.h>
//#include <avr/interrupt.h>

//#define START 1


void IICTransmitMasterWrite(char SlaveAddress, int Data){

  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); //Send start condition

  while (!(TWCR & (1 << TWINT))){} //Wait for TWINT Flag to set. This indicates that START has been transmitted

  //  if((TWSR & 0xF8) != START){
  //    IICError();
  //  }

  TWDR = SlaveAddress | (0<<0);
  TWCR = (1 << TWINT) | (1 << TWEN);
  while (!(TWCR & (1 << TWINT))){}

  TWDR = Data;
  TWCR = (1 << TWINT) | (1 << TWEN);
  while (!(TWCR & (1 << TWINT))){}
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 <<TWSTO);
}





char IICTransmitMasterRead(int SlaveAddress){

  TWCR = (1 << TWINT ) | (1 << TWSTA) | (1 << TWEN);

  while (!(TWCR & (1 << TWINT))){}

  TWDR = SlaveAddress | (1<<0);
  TWCR = (1<<TWINT) | (1<<TWEN) | (0<<TWSTA);
  while (!(TWCR & ( 1 << TWINT))){}
  //This while loop is exiting 
  TWCR = (1<<TWINT) | (1<<TWEA) | (0<<TWSTO) | (0<<TWSTA);
  //  while (!(TWCR & (1<<TWINT))){}

  char temp = TWDR;

  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);

  return temp;

}

//void IICError(){




void TWIInit(void){
  //set SCL to 400Khz
  TWSR = 0x00;
  TWBR = 0x0C;
  //enable TWI
  TWCR = (1<<TWEN);
}

void TWIStart(void){
  TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
  while ((TWCR & (1<<TWINT)) == 0);
}

void TWIStop(void){
  TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
}

void TWIWrite(uint8_t u8data){
  TWDR = u8data;
  TWCR = (1<<TWINT) | (1<<TWEN);
  while ((TWCR & (1<<TWINT)) == 0);
}

uint8_t TWIReadACK(void){
  TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
  while ((TWCR & (1<<TWINT)) == 0);
  return TWDR;
}

uint8_t TWIReadNACK(void){
  TWCR = (1<<TWINT) | (1<<TWEN);
  while ((TWCR & (1<<TWINT)) == 0);
  return TWDR;
}

uint8_t TWIGetStatus(void){
  uint8_t status;
  status = TWSR & 0xF8;
  return status;
}
