#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#include "mef.h"
#include "keypad.h"
#include "timer.h"


int main(void)
{
	uint8_t key;
	
	LCDinit();
	LCDclr();
	LCDcursorOFF();
	KEY_init();
	
	TIMER_Init();
	TIMER_Start(10);
	
	MEF_Init();

	while(1){
		Tec_Check();
		MEF_Check();
		}
}
