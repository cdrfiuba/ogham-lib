#include "../board/board.h"



int main (void)
{
    // inicializamos la placa
    initBoard();
    
    // Bucle infinito
    while(1)
    {
        togglePin(&LED[0]);  // parpadear el led para saber que esta vivo
        _delay_ms(500); // demora para que podamos apreciar el parpadeo
    }
    
    return 0; 
}
