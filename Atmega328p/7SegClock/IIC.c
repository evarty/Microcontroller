#include <avr/io.h>
#include <avr/interrupt.h>

void IICTransmitMasterWrite(char SlaveAddress, int Data){

TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); //Send start condition

while (!(TWCR & (1 << TWINT))){} //Wait for TWINT Flag to set. This indicates that START has been transmitted

if ((TWSR & 0xF8) != START){
  Error(); //Check TWI Status Register and send error if appropriate
}

TWDR = SlaveAddress | (0<<0);
TWCR = (1 << TWINT) | (1 << TWEN);
while (!(TWCR & (1 << TWINT))){}
if ((TWSR & 0xF8) != MT_SLA_ACK){
  Error();
}
TWDR = Data;
TWCR = (1 << TWINT) | (1 << TWEN);
while (!(TWCR & (1 << TWINT))){}
if ((TWSR & 0xF8) != MT_DATA_ACK){
  Error();
}
TWCR = (1 << TWINT) | (1 << TWEN) | (1 <<TWSTO);
}





char IICTransmitMasterRead(int SlaveAddress){

TWCR = (1 << TWINT ) | (1 << TWSTA) | (1 << TWEN);

while (!(TWCR & (1 << TWINT))){}

if ((TWSR & 0xF8) != START){
  Error();
}

TWDR = SlaveAddress | (1<<0);
TWCR = (1<<TWINT) | (1<<TWEN);



}
