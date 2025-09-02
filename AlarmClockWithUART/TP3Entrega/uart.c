
#include "uart.h"
#include "rtc.h"
#include <avr/io.h>
#include <avr/interrupt.h>

volatile char buffer_tx[BUFFER_SIZE];
volatile uint8_t tx_head = 0;
volatile uint8_t tx_tail = 0;
volatile uint8_t tx_busy = 0;
char buffer[BUFFER_SIZE];

//Configura los registros de UART
void UART_init(void){
	UCSR0B = (1<<TXEN0);
	UCSR0C = (1<<UCSZ01) |(1<<UCSZ00);
	UBRR0L = 103; //baud rate =9600bps
}

//Interrupcion que se ejecuta cuando se recibe un BYTE por RX
ISR(USART_RX_vect)
{

	uint8_t received_byte = UDR0;
	if ((received_byte == '\b' || received_byte == 0x7F) && cmd_index > 0) // Manejo de retroceso
	{
		cmd_index--;
	}
	if (received_byte == '\r' || received_byte == '\n') //Proceso el Enter
	{
		buffer_rx[cmd_index] = '\0';
		cmd_index = 0;
		flag_rx = 1; // flag de comando listo para el main loop
		flag_writing=0; //Ya no hay escritura en proceso
		UART_Send_String("\r\n");
	}
	else if (cmd_index < BUFFER_SIZE - 1)
	{
		flag_writing = 1; //Hay escritura en proceso
		if (!(received_byte == '\b' || received_byte == 0x7F)) //Para evitar el guardado de los retrocesos (sino causa comando erroneo si metiste un \b)
		{
			buffer_rx[cmd_index++] = received_byte;
		}
		UART_Send_Char(received_byte);
	}
}

//Interrupcion que se ejecuta cuando el registro de datos esta vacio
ISR(USART_UDRE_vect)
{
	if (tx_tail == tx_head) // buffer vacio
	{
		tx_busy = 0;
		UCSR0B &= ~(1 << UDRIE0); // deshabilita interrupcion
		return;
	}

	UDR0 = buffer_tx[tx_tail];
	tx_tail = (tx_tail + 1) % BUFFER_SIZE;
}

//Envía un string por UART caracter por carácter llamando a UART_Send_Char().
void UART_Send_String(const char *str)
{
	while (*str)
	{
		UART_Send_Char(*str++);
	}
}

//Formatea e imprime por UART la fecha y hora "FECHA: DD/MM/YY HORA: HH:MM:SS\r\n"
UART_send_dateTime(rtc_t current_date)
{
	char buffer[50];
	snprintf(buffer, sizeof(buffer), "FECHA: %02d/%02d/%02d HORA: %02d:%02d:%02d\r\n",
	current_date.dia, current_date.mes, current_date.anio,
	current_date.hora, current_date.min, current_date.seg);
	UART_Send_String(buffer);
}

//Habilita el transmisor UART (bit TXEN0).
void UART_TX_Enable(void)
{
	UCSR0B |= (1 << TXEN0);
}

//Habilita el receptor UART (bit RXEN0).
void UART_RX_Enable(void)
{
	UCSR0B |= (1 << RXEN0);
}

//Habilita la interrupcion por recepción completa (bit RXCIE0).
void UART_RX_Interrupt_Enable(void)
{
	UCSR0B |= (1 << RXCIE0);
}

//Envía un unico byte directamente a UDR0 (modo bloqueante)
void UART_Send_Data(char data)
{
	UDR0 = data;
}

//Agrega un caracter al buffer circular buffer_tx[]:
void UART_Send_Char(uint8_t ch)
{
	uint8_t next_head = (tx_head + 1) % BUFFER_SIZE;

	while (next_head == tx_tail);

	buffer_tx[tx_head] = ch;
	tx_head = next_head;

	// si no hay transmision en curso, iniciar la transmision
	if (!tx_busy)
	{
		tx_busy = 1;
		UCSR0B |= (1 << UDRIE0); // habilitar interrupcion de transmision
	}
}