/**
 * Ejemplo 4
 * Sintesis de onda con PWM.
 * En este ejemplo generamos una señal almacenada en una tabla previamente
 * generada. Se puede medir la salida en el pin 17 (OC2) colocando un filtro
 * pasabajos.
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
    // inicializamos la placa
    initBoard();
    
    // inicializamos el PWM en modo 3(fast PWM), prescaler 1 (fclk) y salida en el pin OC2
    configPWM2(3, 1, interrupcionPWM, 1);
    startPWM2();
    
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
    // Usamos una función especial para leer de la memoria de programa
    pwmVal = pgm_read_byte(&tabla[indice]);
    
    // Establecemos el ancho del pulso
    setPWM2(pwmVal);
    
    // Incrementamos el índice a la próxima muestra
    indice += paso;
    
    // Limitamos el índice al tamaño de la tabla
    indice %= largo;
}
