/*
 * timer.c
 *
 * Created: 7/5/2025 20:50:16
 *  Author: Argota Valentina
 */ 
#include "timer.h"

/**
 * Inicializamos el timer en modo CTC, con un prescaler de 1024 y un valor de OCR0A que interrumpa cada 10ms.
 */

void TIMER_Init(void)
{
    TCCR0A = (1 << WGM01);              // Configurar el timer en modo CTC
    TCCR0B = (1 << CS02) | (1 << CS00); // Configurar el preescalador del timer en 1024
}

/**
 * Iniciamos el timer con un valor de 10ms, que es el tiempo que queremos entre interrupciones.
 * @param valor: el valor en ms que queremos entre interrupciones.
 */
void TIMER_Start(uint8_t valor)
{
    OCR0A = (CICLOS * valor / 1000) - 1; // Se despeja el valor de OCR0A
    TIMSK0 |= (1 << OCIE0A); // Activar la interrupci?n del timer 0
    sei();                   // Habilitar las interrupciones globales
}

