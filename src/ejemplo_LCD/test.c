/**
 * Prueba del LCD.
 **/

#include "../board/board.h"
#include "../board/lcd.h"

// Definimos los pines del micro
static const IOPIN_t databusLCD = {&PORTD, &DDRD, &PIND, 0};  // Port D low nibble
static const IOPIN_t enPinLCD = {&PORTC, &DDRC, &PINC, 1};  // Port C pin 1
static const IOPIN_t rwPinLCD = {&PORTD, &DDRD, &PIND, 6};  // Port D pin 6
static const IOPIN_t rsPinLCD = {&PORTD, &DDRD, &PIND, 5};  // Port D pin 5

int main (void)
{
    // Inicializamos la placa
    initBoard();
    
    // Inicializamos el LCD 
    configLCD(&databusLCD, &enPinLCD, &rwPinLCD, &rsPinLCD);
    
    // Escribimos texto en el LCD
    printLCD("Hola chicos!");
    
    // Bucle infinito
    while(1)
    {
        // Parpadeamos el led
        togglePin(&LED[0]);
        delay_ms(500);
    }
    
    return 0;
}
