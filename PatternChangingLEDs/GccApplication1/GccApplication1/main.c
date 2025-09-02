#include <avr/io.h> // Registros del microcontrolador
#define F_CPU 16000000UL // Defino la frecuencia de oscilador en 8MHz
#include <util/delay.h> // Retardos por software
/* Función main */
int main (void)
{
	/* Setup */
	unsigned char flag = 1; // Seteo Flag utilizada para saltar de secuencia en secuencia, donde un break no es suficiente
	DDRD = 0xFF; // Configuro Puerto D como salida
	DDRC &= ~(1<<PORTC0) | ~(1<<PORTC1); // Configuro bit0 puerto C como entrada
	PORTC |= (1<<PORTC0) | (1<<PORTC1); // Habilito Pull-Up en bit0 puerto C
	DDRB |= (1<<PORTB3) | (1<<PORTB4); // Configuro bit3 y bit4 como salida
	/* Loop */
	while(1){

		while(flag) { //En lugar de depender de solo break para el salto de secuencia utilizo un flag
			if (PINC & (1<<PINC0)) { //Mientras el boton para cambiar de secuencia siga presionado, se va retener la secuencia, para evitar multiples lecturas de un solo pulso
				if (~PINC & (1<<PINC1)){ //Si el boton de imprimir estado esta presionado, imprimo el nro de secuencia, sino imprimo 0
					//PORTB = 0bxxx01xxx;
					PORTB &= ~(1<<4);
					PORTB |= (1<<3); 
					} else {
					//PORTB = 0bxxx00xxx;
					PORTB &= ~((1<<3)|(1<<4));
				}
				PORTD = 0b10101010; // Escribo Port D con patrón de bits
				_delay_ms(100); // Delay de 100 ms
				PORTD = 0b01010101; // Escribo Port D con otro patrón de bits
				_delay_ms(100); // Delay de 100 ms
				
				if (~PINC & (1<<PINC0)){ //Lectura C0
					_delay_ms(300);
					break;
				}
			}
		}
		while(flag){
			if (PINC & (1<<PINC0)) {
				
				for (int i=0 ; i<8 ; i++){
					PORTD = (1 << i);
					_delay_ms(100);
					if (~PINC & (1<<PINC1)){ //Lectura C1
						//PORTB = 0bxxx10xxx;
						PORTB |= (1<<4);
						PORTB &= ~(1<<3);
						} else {
						//PORTB = 0bxxx00xxx;
						PORTB &= ~((1<<3)|(1<<4));
					}
					if (~PINC & (1<<PINC0)){ //Lectura C0
						_delay_ms(300);
						flag=0;
						break;
					}
				}
			}
		}
		flag=1; //Cuando salgo del while porque flag = 0, reseteo el flag en = 1 para quedarme en el siguiente loop hasta que se vuelva a presionar el boton de cambio de secuencia
		while (flag){
			if (PINC & (1<<PINC0)) {
				
				for (int i=7 ; i>0 ; i--){
					PORTD = (1 << i);
					_delay_ms(100);
					if (~PINC & (1<<PINC1)){ //Lectura C1
						//PORTB = 0bxxx11xxx;
						PORTB |= ((1<<3)|(1<<4));
						} else {
						//PORTB = 0bxxx00xxx;
						PORTB &= ~((1<<3)|(1<<4));
					}
					if (~PINC & (1<<PINC0)){ //Lectura C0
						_delay_ms(300);
						flag=0;
						break;
					}
				}
				if (flag==0) { //Para evitar retrasos en el salto de secuencia se chequea aca el flag
					break;
				}
				for (int i=0 ; i<7 ; i++){
					PORTD = (1 << i);
					_delay_ms(100);
					if (~PINC & (1<<PINC1)){ //Lectura C1
						//PORTB = 0bxxx11xxx;
						PORTB |= ((1<<3)|(1<<4));
						} else {
						//PORTB = 0bxxx00xxx;
						PORTB &= ~((1<<3)|(1<<4));
					}
					if (~PINC & (1<<PINC0)){ //Lectura C0
						_delay_ms(300);
						flag=0;
						break;
					}
				}
			}
		}
		flag=1;
	}
	/* Punto de finalización del programa (NO se debe llegar a este lugar) */
	return 0;
}