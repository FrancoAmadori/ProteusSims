
#ifndef TIMER_H
#define TIMER_H

#define F_CPU 16000000UL
#define PRESCALER 1024
#define CICLOS (F_CPU / PRESCALER) // Frecuencia de funcionamiento sobre el prescaler

#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

uint16_t Contador;
uint8_t flag_tick_1s;

void TIMER_Init(void);               // Inicializa el Timer

#endif