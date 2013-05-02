/**
 * Ejemplo 6
 * Prueba del ADC midiendo el LDR de la placa.
 **/

#include "../board/board.h"

void barraLed(uint8_t val);

int main (void)
{
    // Inicializamos la placa
    initBoard();
    
    //setADCChannel(0);
    uint8_t a=0;
    int8_t s=1;
    
    // Bucle infinito
    while(1)
    {
        //togglePin(&LED[0]);  // parpadear el led para saber que esta vivo

        //delay_ms(100);  // demora para que podamos apreciar el parpadeo
        //getADCValue();
        //if (a==255 && s == 1) s = -s;
        //if (a==0 && s == -1) s = -s;
        //a += s;
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
