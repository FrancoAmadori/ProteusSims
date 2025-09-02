
#include <avr/io.h>
#include "main.h"
#include "timer.h"
#include "utils.h"
#include "uart.h"

uint8_t contador_alarma;
volatile uint8_t flag_ON = 0;
volatile uint8_t flag_ALARMA = 0;

volatile uint8_t ESPERANDO_SET_TIME = 0; // Variable para esperar el tiempo de respuesta del usuario
volatile uint8_t ESPERANDO_SET_ALARM = 0; // Variable para esperar el tiempo de respuesta del usuario
volatile uint8_t ESPERANDO_COMANDO = 0; // Variable para esperar el tiempo de respuesta del usuario
volatile uint8_t VALID = 0;

int main()
{
    // Inicializaciones
    i2c_init();

    UART_init();
    UART_TX_Enable();
    UART_RX_Enable();
	UART_RX_Interrupt_Enable();
    
    TIMER_Init(); 

    sei();

    UART_Send_String("Bienvenido. Opciones ON OFF SET DATE SET ALARM\r\n");


    while (1)
    {

        if (flag_rx)
        {
            ESPERANDO_COMANDO=1;
            if (ESPERANDO_SET_TIME || ESPERANDO_SET_ALARM) {
                if (ESPERANDO_SET_TIME)
                {
                    update_dateTime(buffer_rx);
                }
                if (ESPERANDO_SET_ALARM)
                {
                    update_alarm(buffer_rx);
                }
            }
            // Lo que recibi es comando (PUES NO ESPERABA DATO)
            else {
                procesar_comando(buffer_rx);
            }
			ESPERANDO_COMANDO=0;
            flag_rx = 0;
            
        }

        if (flag_tick_1s)
        {
			
            rtc_getDateTime(&current_date);
            if ((flag_ON)&&(!ESPERANDO_SET_ALARM)&&(!ESPERANDO_SET_TIME)&&(!flag_ALARMA)&&(!ESPERANDO_COMANDO)&&(!flag_writing)){ 
				//Mientras se esté en uno de los estados de espera, alarma o escritura: no se imprime fecha/horario
				
                UART_send_dateTime(current_date); // Formatea y manda: "FECHA: DD/MM/YY HORA: HH:MM:SS\r\n"
            }

            //Si hay una alarma seteada entonces contador_alarma != 0 
            if(contador_alarma != 0)
            {
                verificar_alarma(&current_date,&alarm);   // Envia "ALARMA\r\n" si corresponde
            }
            
            flag_tick_1s = 0;
        }
    }
}

void procesar_comando(const char *buffer_rx)
{	
	
    if (strcmp(buffer_rx, "ON") == 0)
    {
        flag_ON = 1;
        UART_Send_String(">> Modo ON\r\n");
    }
    else if (strcmp(buffer_rx, "OFF") == 0)
    {
        flag_ON = 0;
        UART_Send_String(">> Modo OFF\r\n");
    }

    else if (strcmp(buffer_rx, "SET DATE") == 0)
    {
        ESPERANDO_SET_TIME = 1;
        UART_Send_String("Ingrese fecha y hora con el siguiente formato: DD/MM/YY HH:MM\r\n");
    }
    else if (strcmp(buffer_rx, "SET ALARM") == 0)
    {
        ESPERANDO_SET_ALARM = 1;
        UART_Send_String("Ingrese la hora de la alarma con el siguiente formato: HH:MM\r\n");
    }
    else
    {
        UART_Send_String(">> Comando no reconocido\r\n");
		
    }
	
}



void update_dateTime(const char *buffer_rx){

    VALID = is_valid_date((char *)buffer_rx); // DD/MM/YY HH:MM
    if(VALID)
    {   
        rtc_setDateTime(&toSet_date); // Setea la fecha y hora

        UART_Send_String("Fecha y hora actualizadas correctamente.\r\n");
        ESPERANDO_SET_TIME = 0; // Reseteamos la variable de espera
    }
    else 
    {
        UART_Send_String("Formato incorrecto. (DD/MM/YY HH:MM)\r\n");
    }
}

void update_alarm(const char *buffer_rx){
    VALID = is_valid_time((char *)buffer_rx, &alarm);    
    if(VALID)
    {
        ESPERANDO_SET_ALARM = 0;
        contador_alarma = 5; // Reseteamos el contador de alarma
        UART_Send_String("Hora de alarma actualizada correctamente. \r\n");
    }
    else  
    {
        UART_Send_String("Formato incorrecto. (HH:MM)\r\n");
    }
}


void verificar_alarma(rtc_t *current_date, rtc_t *alarm) {
    
    if (current_date->hora == alarm->hora && current_date->min == alarm->min) {
		flag_ALARMA=1; //Mientras suena alarma
        contador_alarma--;
        UART_Send_String("ALARMA\r\n");
		if (contador_alarma==0)
			flag_ALARMA=0; //Termino de sonar
    }
}



 

