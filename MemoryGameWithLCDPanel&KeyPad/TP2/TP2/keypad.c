#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>

// Pines columnas - entradas con pullup
#define COL0 PD3
#define COL1 PD5
#define COL2 PD4
#define COL3 PD2

// Pines filas - salidas una a la vez
#define F0 PB4
#define F1 PB3
#define F2 PB0
#define F3 PD7

// Mapeo de las teclas
const char keymap[16] = {
	'1','2','3','A',
	'4','5','6','B',
	'7','8','9','C',
	'*','0','#','D'
};


void KEY_init(void)
{

	// Si bien configuramos los pines correspondientes a las filas como entrada, en cada escaneo se configura una fila a la vez como salida.


	// Columnas (entradas con pull-up)
	DDRD &= ~((1 << COL0) | (1 << COL1) | (1 << COL2) | (1 << COL3));// pongo en 0 los bits de direccion -> los config como entrada
	PORTD |= (1 << COL3) | (1 << COL0) | (1 << COL2) | (1 << COL1);  //activamos pull-up

	// Filas (salidas que activamos una a la vez)
	DDRB &= ~((1 << F2) | (1 << F1) | (1 << F0)); //pongo en 0 los bits de direccion -> los config como entrada
	PORTB |= (1 << F2) | (1 << F1) | (1 << F0); // activo el pull-up
	DDRD &= ~(1 << F3); // pongo en 0 el bit de direcci?n
	PORTD |= (1 << F3); // le activo el pull-up
}


static uint8_t KeypadUpdate(void)
{
	uint8_t f, c;


	PORTD |= (1<<COL0) | (1<<COL1) | (1<<COL2) | (1<<COL3);


	for (f=0;f<4;f++)
	{
		
		// seteo todas las filas como entradas
		DDRD &= ~(1 << F3);
		DDRB &= ~((1 << F0) | (1 << F1) | (1 << F2));

		switch(f) // Activamos una fila como salida y la ponemos en 0
		{
			case 0:
			DDRB |= (1 << F0);
			PORTB &= ~(1 << F0);
			break;
			case 1:
			DDRB |= (1 << F1);
			PORTB &= ~(1 << F1);
			break;
			case 2:
			DDRB |= (1 << F2);
			PORTB &= ~(1 << F2);
			break;
			case 3:
			DDRD |= (1 << F3);
			PORTD &= ~(1 << F3);
			break;
		}
		
		_delay_us(5);

		for(c = 0; c <4; c++)
		{
			switch(c) // Recorremos las columnas para ver si alguna tecla fue presionada
			{
				case 0:
				if (!(PIND & (1 << COL0)))
				return keymap[f * 4 + c];
				break;
				case 1:
				if (!(PIND & (1 << COL1)))
				return keymap[f * 4 + c];
				break;
				case 2:
				if (!(PIND & (1 << COL2)))
				return keymap[f * 4 + c];
				break;
				case 3:
				if (!(PIND & (1 << COL3)))
				return keymap[f * 4 + c];
				break;
			}
		}
	}

	return 0xFF;
}

uint8_t KEYPAD_Scan (uint8_t *pkey)
{
	static uint8_t Old_key, Last_valid_key=0xFF; // no hay tecla presionada;
	uint8_t Key;
	Key = KeypadUpdate();
	if(Key==0xFF){
		Old_key=0xFF;    // no hay tecla presionada
		Last_valid_key=0xFF;
		return 0;
	}
	if(Key==Old_key) { //2da verificacion
		if(Key!=Last_valid_key){ //evita multiple deteccion
			*pkey=Key;
			Last_valid_key = Key;
			return 1;
		}
	}
	Old_key=Key; //1era verificaci?n
	return 0;
}