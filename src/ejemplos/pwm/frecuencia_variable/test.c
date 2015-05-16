/**
 * Sintesis de onda con PWM permitiendo modificar la frecuencia con el pulsador.
 * Se puede medir la salida en el pin 17 (OC2) colocando un filtro pasabajos.
 **/

#include <avr/pgmspace.h>
#include "board/board.h"

// Declaración de las funciones
void interrupcionPWM(void);
void interrupcionBoton(void);

// La variable que contendrá el ancho del pulso es "volatile" porque puede
// cambiar repentinamente en cualquier momento cuando se presione el botón.
volatile uint8_t pwmVal=0;

// Tabla con los valores de la señal muestreada
// Tenemos que indicarle que la tabla se almacena en la memoria del programa
const uint8_t tabla[] PROGMEM = {128,153,178,200,220,236,247,254,255,251,242,228,211,189,166,140,115,89,66,44,27,13,4,0,1,8,19,35,55,77,102,127};

// Tamaño de la tabla
const uint8_t largo = sizeof(tabla);

// Incremento del índice
uint8_t paso=1;

// Variable para indexación de la tabla
uint8_t indice=0;


int main (void)
{
    // Inicializamos la placa
    initBoard();
    
    // Inicializamos el PWM en modo 3(fast PWM), prescaler 1 (fclk) y salida en el pin OC2
    configPWM2(3, 1, interrupcionPWM, 1);
    startPWM2();
    
    // Configuramos la función que será llamada cuando se presione el botón
    // 5 ms para filtrar los rebotes
    initButton(interrupcionBoton, 5);
    
    // Bucle infinito
    while(1)
    {
        togglePin(&LED[0]);  // parpadear el led para saber que esta vivo
        delay_ms(100);  // demora para que podamos apreciar el parpadeo
    }
    
    return 0;
}

// Función que se ejecuta al inicio de cada período del PWM
void interrupcionPWM(void)
{
    // contador
    static uint8_t c = 0;
    
    // Usamos una función especial para leer de la memoria de programa
    pwmVal = pgm_read_byte(&tabla[indice]);
    
    // Establecemos el ancho del pulso
    setPWM2(pwmVal);
    
    if (c == 0)  // Cuando el contador llega a cero, avanzamos el índice de la tabla
    {
        indice++;
        c = paso;  // Cargamos la cantidad de períodos de PWM que queremos mantener el mismo valor del la tabla
    }
    
    c--;
    
    // Limitamos el índice al tamaño de la tabla
    indice %= largo;
}

// Función que se ejecuta cada vez que se presiona el botón
void interrupcionBoton(void)
{
    paso+=1;
}
