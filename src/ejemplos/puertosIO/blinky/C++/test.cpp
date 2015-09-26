/**
 * Ejemplo 1
 * Probar la placa encendiendo un led.
 * Hacemos parpadear un led.
 **/
 
#include "board/board.h"
#include "lib/delay.h"

class foo
{
	int bar;
};

int main (void)
{
    // inicializamos la placa
    initBoard();
    
    // Bucle infinito
    while(1)
    {
        togglePin(&LED[0]);  // parpadear el led para saber que esta vivo
        delay_ms(500); // demora para que podamos apreciar el parpadeo
    }
    
    return 0; 
}
