
#ifndef _MAIN_H
#define _MAIN_H
#include <avr/io.h>
#include "rtc.h"
#include "i2c.h"

void procesar_comando(const char *buffer_rx);
void update_time(const char *buffer_rx);
void update_alarm(const char *buffer_rx);

rtc_t current_date;  // Estructura para almacenar la fecha y hora actual
rtc_t toSet_date;  // Estructura para almacenar la fecha y hora a setear
rtc_t alarm;   // Estructura para almacenar la fecha y hora de la alarma


#endif