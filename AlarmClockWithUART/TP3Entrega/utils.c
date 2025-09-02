
#include "utils.h"
#include "main.h"



uint8_t is_valid_time(const char *str, rtc_t *alarm) {
	if (strlen(str) != 5) return 0; //HH:MM

	if (str[2] != ':')
		return 0;

	for (int i = 0; i <5; i++) {
		if (i == 2) continue;
		if (!isdigit((unsigned char)str[i]))
		return 0;
	}
	uint8_t hour = (str[0] - '0') * 10 + (str[1] - '0');
	uint8_t min  = (str[3] - '0') * 10 + (str[4] - '0');

	if (hour > 23 || min > 59)
	return 0;
	
	
	alarm->hora = hour;
	alarm->min = min;
	alarm->seg = 0; // Por defecto, los segundos son 0
	return 1;
}

uint8_t isdigit(char c){
	if(c>'0' && c<'9'){
		return 1;
	}
	return 0;
}

uint8_t is_valid_date(const char *date_str) //DD/MM/YY HH:MM
{
	// Implementar la validación de la fecha y hora
	if (strlen(date_str) != 14) {
		return 0; // Longitud incorrecta
	}
	if (date_str[2] != '/' || date_str[5] != '/' || date_str[8] != ' ' || date_str[11] != ':') {
		return 0; // Formato incorrecto
	}
	uint8_t day   = (date_str[0] - '0') * 10 + (date_str[1] - '0');
	uint8_t month = (date_str[3] - '0') * 10 + (date_str[4] - '0');
	uint8_t year  = (date_str[6] - '0') * 10 +(date_str[7] - '0');
	uint8_t hour  = (date_str[9]  - '0') * 10 + (date_str[10] - '0');
	uint8_t min   = (date_str[12] - '0') * 10 + (date_str[13] - '0');

	// Validar rango de valores
	if (day < 1 || day > 31 || month < 1 || month > 12 || hour > 23 || min > 59) {
		return 0;
	}

	// Validar días del mes
	if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {
		return 0; // Abril, Junio, Septiembre, Noviembre tienen 30 días
	}
	if (month == 2) {
		int full_year = 2000 + year; // asumimos siglo 21
		// Febrero: 28 días en anios no bisiestos, 29 en bisiestos
		if ((full_year % 4 == 0 && full_year % 100 != 0) || (full_year % 400 == 0)) {
			if (day > 29) return 0; // Anio bisiesto
			} else {
			if (day > 28) return 0; // No bisiesto
		}
	}
	toSet_date.dia = day;
	toSet_date.mes = month;
	toSet_date.anio = year;
	toSet_date.hora = hour;
	toSet_date.min = min;
	toSet_date.seg = 0; // Por defecto, los segundos son 0
	return 1;
}