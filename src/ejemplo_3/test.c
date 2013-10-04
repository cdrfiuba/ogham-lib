/**
 * Ejemplo 3
 * Uso del PWM + pulsador
 * En este ejemplo empleamos el pulsador para aumentar el ancho de pulso.
 * Se puede medir la salida en el pin 17 (OC2) con un osciloscopio.
 **/

#include "board/board.h"


void interrupcionBoton(void);

// La variable que contendrá el ancho del pulso es "volatile" porque puede
// cambiar repentinamente en cualquier momento cuando se presione el botón.
volatile uint8_t pwmVal=0;


int main (void)
{
    // inicializamos la placa
    initBoard();
    
    // configuramos la función que será llamada cuando se presione el botón
    // 5 ms para filtrar los rebotes
    initButton(interrupcionBoton, 5);
    
    // inicializamos el PWM en modo 3(fast PWM), prescaler 2 (fclk/8) y salida en el pin OC2
    configPWM2(3, 2, NULL, 1);
    startPWM2();
    
    // Bucle infinito
    while(1)
    {
        togglePin(&LED[0]);  // parpadear el led para saber que esta vivo
        delay_ms(100);  // demora para que podamos apreciar el parpadeo
        setPWM2(pwmVal);  // establecemos el ancho del pulso
    }
    
    return 0;
}

void interrupcionBoton(void)
{
    pwmVal+=10;  // incrementamos el ancho del pulso del PWM
}
