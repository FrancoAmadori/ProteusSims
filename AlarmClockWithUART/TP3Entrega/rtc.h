
//tengo que tener variables que me guarden la fecha y la hora
//no se si no conviene poner todo en un solo registro y fue

//como cuando leo el puntero se va moviendo de direcci?n a direccion
//pongo las direcciones a leer de manera consecutiva
#ifndef RTC_H
#define RTC_H

#include <avr/io.h>
#include <stdint.h>
#include "i2c.h"

#define DS3231_SEG 0x00
#define DS3231_MIN 0x01
#define DS3231_HORA 0x02

#define DS3231_DIA 0x04
#define DS3231_MES 0x05
#define DS3231_YEAR 0x06


typedef struct{
	uint8_t seg;
	uint8_t min;
	uint8_t hora;
	uint8_t dia;
	uint8_t mes;
	uint8_t anio;
}rtc_t;

void rtc_setDateTime(rtc_t *dateTime);
void rtc_getDateTime(rtc_t *dateTime);

#endif