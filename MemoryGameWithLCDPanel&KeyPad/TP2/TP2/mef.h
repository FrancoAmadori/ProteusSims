#include "lcd.h"
#include "keypad.h"
#include <stdlib.h> 
#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <time.h>
#include <stdio.h>

typedef enum {
	INICIO,
	MOSTRAR_CLAVE,
	LECTURA,
	ERROR,
	CORRECTO,
	VICTORIA,
	DERROTA
	}Estados;

// <-- Prototipos de funciones que controlan la MEF -->//	
void MEF_Init(void);
Estados MEF_estado_actual(void);
void MEF_Update(void);
void MEF_Check(void);

void consulto_teclado(void);
void Tec_Check(void);
//<-- Prototipos de funciones para cada estado -->//
void inicio(void);
char* generar_clave(const char *, uint8_t);
void mostrar_clave(void);
void lectura(void);
void error(void);
void correcto(void);	
void victoria(void);
void derrota(void);