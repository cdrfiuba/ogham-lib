#include "utils.h"
#include "board.h"
#include <util/delay.h>


int main (void)
{
    initBoard();
    
    // Bucle infinito
    while(1)
    {
        togglePin(&LED[2]);  // parpadear el led para saber que esta vivo
        _delay_ms(500); // demora para que podamos apreciar el parpadeo
    }
    
    return 0; 
}
