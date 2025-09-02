
#include "rtc.h"


//Convierte un número decimal a formato BCD , que es el que usa el RTC DS3231.
uint8_t int_a_bcd(uint8_t n)
{
	return (((n / 10) << 4)) | (n % 10);

}

//Escribe hora y fecha completa al RTC
void rtc_setDateTime(rtc_t *dateTime)
{
	i2c_start();
	i2c_write(0xD0); //Modo escritura

	i2c_write(DS3231_SEG); //Me ubico en la direccion de segundos

	i2c_write(int_a_bcd(dateTime->seg));
	i2c_write(int_a_bcd(dateTime->min)); //i2c_write mueve el puntero a medida que escribo
	i2c_write(int_a_bcd(dateTime->hora));
	
	i2c_write(0); // 0x03 Día de la semana (lo dejamos en 0)
	
	i2c_write(int_a_bcd(dateTime->dia));
	i2c_write(int_a_bcd(dateTime->mes));
	i2c_write(int_a_bcd(dateTime->anio));

	i2c_stop();
}

// --> Lee hora y fecha completa del RTC:
void rtc_getDateTime(rtc_t *dateTime)
{

	i2c_start();
	i2c_write(0xD0); //Modo escritura
	i2c_write(DS3231_SEG); //Me ubico en la dir de segundos
	i2c_stop();

	i2c_start();
	i2c_write(0xD1); //Modo lectura

	dateTime->seg = i2c_read(0); //Mando un ACK
	dateTime->seg = ((((dateTime->seg & 0x70) >> 4) * 10) + (dateTime->seg & 0x0F));

	dateTime->min = i2c_read(0);
	dateTime->min = ((((dateTime->min & 0x70) >> 4) * 10) + (dateTime->min & 0x0F));

	dateTime->hora = i2c_read(0);
	dateTime->hora = (((dateTime->hora & 0x30) >> 4) * 10) + (dateTime->hora & 0x0F);

	i2c_read(0); // Descarta el campo de día de la semana (0x03)
	
	dateTime->dia = i2c_read(0);
	dateTime->dia = ((((dateTime->dia) & 0x30) >> 4) * 10) + ((dateTime->dia) & 0x0F);
	dateTime->mes = i2c_read(0);
	dateTime->mes = ((((dateTime->mes) & 0x10) >> 4) * 10) + ((dateTime->mes) & 0x0F);
	dateTime->anio = i2c_read(1); //Mando un NACK
	dateTime->anio = ((((dateTime->anio) & 0xF0) >> 4) * 10) + ((dateTime->anio) & 0x0F);
	i2c_stop();
}
