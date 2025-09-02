#include "timer.h"

void TIMER_Init(void)
{
    TCCR0A = (1 << WGM01);              // Modo CTC
    TCCR0B = (1 << CS01) | (1 << CS00); // Prescaler = 64
    OCR0A = 249;                        // Valor para generar interrupción cada 1 ms, asumo que F_CPU = 16MHz
	TIMSK0 |= (1 << OCIE0A);            // Habilitar interrupción por comparación
}


//Asi se ejecuta cada 1 ms
ISR(TIMER0_COMPA_vect)
{
	++Contador;
    if(Contador == 1000)
    {
        flag_tick_1s = 1; // Indicamos que ha pasado 1 segundo
        Contador = 0; 
    }
}