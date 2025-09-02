
#include "i2c.h"

void i2c_init(void){
	
	TWSR = 0x00; //Seteo los bits del prescalador en 0
	TWBR =152; //La frecuencia en 16Mhz 
	TWCR=0x04; //Se habilita el modulo TWI
}

void i2c_start(void){
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	while((TWCR & (1<<TWINT)) == 0); //Espera confirmacion
}

void i2c_write(unsigned char data){
	TWDR = data; //Copio dato a enviar TWDR
	TWCR = (1<<TWINT) | (1<<TWEN); //TWEN y TWINT en 1 para habilitar transmision
	while((TWCR & (1<<TWINT)) == 0 ); //Espera a que se active TWINT (se termino la transmision)
}


unsigned char i2c_read(unsigned char isLast){
	
	if(isLast == 0) //mando el ACK
		TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA); //mando el ACK
	else
		TWCR = (1<<TWINT) | (1<<TWEN); //Mando el notACK
	while((TWCR & (1<<TWINT)) == 0 );
	return TWDR;
	
}

void i2c_stop()
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO); 
}

	

