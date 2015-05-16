/**
 * Prueba de la UART enviando asteriscos!
 **/

#include "board/board.h"

// función principal
int main (void)
{
    // Inicializamos la placa
    initBoard();
    
    // configuramos la UART a 9600 bps
    configUart(9600);
    
    // Bucle infinito
    while(1)
    {
        // enviamos un byte por la UART
        setUartTx('*');
        
        // parpadeamos el led para saber que estamos vivos!
        togglePin(&LED[0]);
        
        // esperamos medio segundo para que no vaya tan rápido!
        delay_ms(500);
    }
    
    return 0;
}
