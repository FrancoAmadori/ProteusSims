 
#ifndef _UART_H
#define _UART_H
#include <stdint.h>


#define BUFFER_SIZE 128

volatile uint8_t buffer_rx[BUFFER_SIZE];
volatile uint8_t cmd_index;
volatile uint8_t flag_rx;
volatile uint8_t flag_writing;

void UART_init(void);
void UART_Send_String(const char *str);
void UART_send_dateTime(rtc_t);

#endif