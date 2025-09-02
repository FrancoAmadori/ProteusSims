/*
 * timer.h
 *
 * Created: 7/5/2025 20:50:31
 *  Author: Argota Valentina
 */ 
#ifndef TIMER_H
#define TIMER_H

#define F_CPU 16000000UL
#define PRESCALER 1024
#define CICLOS (F_CPU / PRESCALER)  // Frecuencia de funcionamiento sobre el prescaler
#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>


void TIMER_Init(); // Inicializa el Timer
void TIMER_Start(uint8_t valor); // Inicia el Timer

#endif