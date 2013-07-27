/**
 * Ejemplo 6
 * Prueba del ADC midiendo el LDR de la placa y mostrando el valor con los LEDs.
 **/

#include "../board/board.h"

void barraLed(uint8_t val);

int main (void)
{
    // Inicializamos la placa
    initBoard();
    
    // Bucle infinito
    while(1)
    {
        barraLed(getADCValue());
    }
    
    return 0;
}

// Función que ilumina la barra de leds según el valor del parámetro.
void barraLed(uint8_t val)
{
    if (val > 255/5) clearPin(&LED[3]); else setPin(&LED[3]);
    if (val > 255/5*2) clearPin(&LED[2]); else setPin(&LED[2]);
    if (val > 255/5*3) clearPin(&LED[1]); else setPin(&LED[1]);
    if (val > 255/5*4) clearPin(&LED[0]); else setPin(&LED[0]);
}
