/**
 * Prueba del LCD.
 **/

#include "board/board.h"
#include "mods/lcd.h"

// Definimos los pines del micro
static const IOPIN_t databusLCD = definePin(D, 0);  // Port D low nibble
static const IOPIN_t enPinLCD = definePin(C, 1);  // Port C pin 1
static const IOPIN_t rwPinLCD = definePin(D, 6);  // Port D pin 6
static const IOPIN_t rsPinLCD = definePin(D, 5);  // Port D pin 5

int main (void)
{
    // Inicializamos la placa
    initBoard();
    
    // Inicializamos el LCD 
    configLCD(&databusLCD, &enPinLCD, &rwPinLCD, &rsPinLCD);
    
    // Escribimos texto en el LCD
    printLCD("Wake up Neo");
    
    // Bucle infinito
    while(1)
    {
        // Parpadeamos el led
        togglePin(&LED[0]);
        delay_ms(500);
    }
    
    return 0;
}
