/**
 * Ejemplo 2
 * Uso del PWM
 * En este ejemplo configuramos el PWM e incrementamos paulatinamente el ancho
 * del pulso. Se puede medir la salida en el pin 17 (OC2) con un osciloscopio.
 **/

#include "../board/board.h"
#include <stdlib.h>


int main (void)
{
    uint8_t x = 0;
    
    // inicializamos la placa
    initBoard();
    
    // inicializamos el PWM en modo 3(fast PWM), prescaler 2 (fclk/8) y salida en el pin OC2
    initPWM2(3, 2, NULL, 1);
    startPWM2();
    
    // Bucle infinito
    while(1)
    {
        togglePin(&LED[0]);  // parpadear el led para saber que esta vivo
        _delay_ms(100);  // demora para que podamos apreciar el parpadeo
        setPWM2(x++);  // incrementamos el ancho del pulso
    }
    
    return 0; 
}
