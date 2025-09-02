#include "timer.h"
#include "mef.h"
#include <string.h>
#define NUM_PALABRAS 25

static Estados ESTADO_ACTUAL;

// <------- variables relacionadas con las claves ------->

char *clave_generada; 

static volatile uint8_t existe_clave_generada = 0; 

const char* palabras[] = {
	"Arbol", "Boton", "CDyMC", "ClavE", "Facil", "Gafas", "Hojas",
	"LiBro", "Lanza", "Nieve", "PeRro", "PecES", "PiAno", "PrYKe", "RUEDa",
	"SERIE", "SalUd", "Salud", "Silla", "Tecla", "Valor", "Verde",
	"YnHRz", "hARdD", "silla"
};


// <------- variables relacionadas con  el timer ------->

static volatile uint8_t flag = 0; 
static volatile uint8_t contadorMEF = 0;
static uint8_t State_Call_Count = 0;
static uint8_t randSeed = 0;
static uint16_t Time_count = 0; //contador de segundos en los que tardamos en ganar el juego

// <------- variables relacionadas con el keypad ------->
static uint8_t key; // variable para almacenar la tecla presionada
static volatile uint8_t flagT = 0;
static volatile uint8_t contadorTec = 0;
// <------- variables del juego ------->

static uint8_t errores = 0; 
static uint8_t correctos = 0; 
static volatile uint8_t entero = 0; //cualquier cosa cambiar

// <------- AGREGADO ------->
volatile uint8_t columna_i = 0;
volatile uint8_t tecla_actual = 0;
volatile uint8_t tecla_disponible = 0;

void consulto_teclado(void) {
	uint8_t key;
	if (KEYPAD_Scan(&key)) {
		tecla_actual = key; //me guardo la tecla ingresada
		tecla_disponible = 1; //Pongo en '1' para avisar que se presiono una tecla
	}
}

char *generar_clave(const char *palabra, uint8_t size)
{
	return (char *)palabras[randSeed % size];
}


void MEF_Init(void) {
	ESTADO_ACTUAL = INICIO;	
	LCDGotoXY(0,0); 
	LCDstring("Presione *",10); 
	State_Call_Count = 0; 
	existe_clave_generada = 0; 
}


// Esto se ejecuta cada 10ms
ISR(TIMER0_COMPA_vect)
{	
	randSeed++; //Cada 10 ms cambiar la semilla para la seleccion pseudoaleatoria de la clave
	if(++contadorTec == 5){
		flagT= 1;
		contadorTec = 0;
	}
	if(++contadorMEF == 10){ // Esto se ejecuta cada 100ms
		flag = 1;
		contadorMEF = 0;
	}
}

Estados MEF_estado_actual(void) {
	return ESTADO_ACTUAL;
}

void MEF_Update(void)
{
	State_Call_Count++; 
	//consulto_teclado(); 
	switch(ESTADO_ACTUAL){
		case INICIO:
			inicio();
		break;
		case MOSTRAR_CLAVE:
			mostrar_clave();
			break;
		case LECTURA:
			Time_count++;
			lectura();
			break;
		case ERROR:
			 Time_count++;
			 error();
			 break;
		case CORRECTO:
			Time_count++;
			correcto();
			break;
		case VICTORIA:
			victoria();
			break;
		case DERROTA:
			derrota();
			break;
	}
}


void MEF_Check()
{
	// Si pasaron 100ms, llamo a la MEF
	if (flag)
	{				
		flag = 0;	 
		MEF_Update(); 
	}
}

void Tec_Check()
{
	// Si pasaron 50ms, llamo a la MEF
	if (flagT)
	{
		flagT = 0;
		consulto_teclado();
	}
}
 void inicio(){
	 if (tecla_disponible && tecla_actual == '*') {
		 ESTADO_ACTUAL = MOSTRAR_CLAVE;
		 State_Call_Count = 0;
		 existe_clave_generada = 0;
		 LCDclr();
		 LCDGotoXY(0, 0);
		 tecla_disponible = 0;
	 }
 }


void mostrar_clave()
{
	
	if (!existe_clave_generada) // Si no se generó la clave 
	{
		clave_generada = generar_clave(palabras,NUM_PALABRAS);
		LCDstring(clave_generada, 5); 
		existe_clave_generada = 1;

		// Se reinicia los contadores involucrados en el juego 
		correctos=0; 
		errores=0;
		entero=0;

	}
	else
	{
		if (State_Call_Count > 20) // Si ya pasaron 2 segundos
		{
			LCDclr();
			LCDGotoXY(6, 0);
			LCDstring("Errores: ",9);
			LCDsendChar((errores+48));	//Envia nro de errores en ascii	 
			LCDGotoXY(0, 1);		 
			ESTADO_ACTUAL = LECTURA; 
			State_Call_Count = 0;	
			Time_count = 0;
		}
	}
}

void lectura() {
	if (tecla_disponible) {
		tecla_disponible = 0; //Para evitar guardar la misma tecla 2 veces

		if ((tecla_actual >= '0') && (tecla_actual <= '9')) { //Si presionamos una tecla y esta entre los valores '0' y '9'
			entero = (entero * 10) + (tecla_actual - '0');  //Obtenemos el valor entero de la tecla presionada
			LCDGotoXY(columna_i, 1); //En la primera columna = 0, asi q me posiciono en la fila 1 columna 0
			LCDsendChar(tecla_actual);
			columna_i++; //esto lo agregue para que no se me pisen los valores cuando los voy ingresando, sino escribo siempre uno arriba de otro
		}
		else if (tecla_actual == '#') {
			LCD_LimpiarLinea(1);
			LCDGotoXY(0, 1); //columna, fila
			columna_i = 0;  //una vez que ingrese lo q queria ingresar vuelvo a escribir en la primera columna
			if (clave_generada[correctos] == entero) { // Si el ASCII ingresado corresponde al ASCII de la clave generada
				ESTADO_ACTUAL = CORRECTO;
				} else { // Si el ASCII ingresado NO corresponde al ASCII de la clave generada
				
				ESTADO_ACTUAL = ERROR; 
			}
			entero = 0;  //Lo vuelvo a inicializar para el caracter que sigue
		}
	}
}

void correcto()
{   
	LCD_LimpiarLinea(1);
	LCDGotoXY(correctos, 0); 
	LCDsendChar(clave_generada[correctos]);
	correctos++; 
	if (correctos < 5) // Todavia no finalizó el juego
	{
		ESTADO_ACTUAL = LECTURA; 
	}
	else  // Finalizó el juego
	{
		ESTADO_ACTUAL = VICTORIA;
		State_Call_Count = 0; // Reiniciamos el contador de estados
	}
}



void error()
{
	errores++; 
	LCD_LimpiarLinea(1); // Limpiamos la segunda línea del LCD
	if(errores < 3) // Si no se llegó al límite de errores
	{
		LCDGotoXY(15, 0);
		LCDsendChar((errores+48)); //Envia nro de errores en ascii
		ESTADO_ACTUAL = LECTURA; // Volvemos al estado de lectura
	} 
	else // Si se llegó al límite de errores 
	{
		ESTADO_ACTUAL = DERROTA; 
		LCDclr(); 
	}
	LCDGotoXY(0, 1); //Reubicar el cursor
	State_Call_Count = 0;
}

void derrota()
{

	static uint8_t mostrar_mensaje = 0;

	if(!mostrar_mensaje)
	{
		LCDstring("Derrota :c", 10);
		mostrar_mensaje = 1; 
	}
	else if(State_Call_Count > 50) // si pasaron 5 segundos
		{
			LCDclr();
			ESTADO_ACTUAL = INICIO;
			mostrar_mensaje = 0; 
			MEF_Init();
		}
}

void victoria()
{

	static uint8_t mostrar_mensaje = 0;

	if(!mostrar_mensaje)
	{
		LCDclr();
		LCDGotoXY(1, 0); //columna, fila, aca lo movi a la columna 2 pq no se pq el clr me borra el primer caracter
		LCDstring("Victoria :D", 11);
		LCDGotoXY(0, 1); 
		Time_count /= 10; 
		LCDstring("Tiempo: ",8);
		
		char buffer[6]; // Suficiente para valores de hasta 65535
		itoa(Time_count, buffer, 10);
		LCDstring(buffer, strlen(buffer)); //muestro el tiempo
		mostrar_mensaje = 1; 
	}
	else if(State_Call_Count > 50) // si pasaron 5 segundos
		{
			LCDclr();
			ESTADO_ACTUAL = INICIO; 
			mostrar_mensaje = 0;
			MEF_Init();
		}
}