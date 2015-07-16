/**
 * Ejemplo
 * Uso del timer 0.
 **/

#include "board/board.h"

const IOPIN_t salida = definePin(B, 0);

void time(void);

int main (void)
{
    // inicializamos la placa
    initBoard();
    
    // configuramos el pin como salida
    configPinOut(&salida);

    // configuramos el timer0 con prescaler 3 = fclk/64; cada vez que llegue a 256 cuentas llamará a la interrupción
    configTimer0(3, time);
    
    // arrancamos el timer
    startTimer0();
    
    // Bucle infinito
    while(1)
    {
        togglePin(&LED[0]);  // parpadear el led para saber que esta vivo
        delay_ms(500);  // demora para que podamos apreciar el parpadeo
    }
    
    return 0; 
}

void time(void)
{
    togglePin(&salida);
}
