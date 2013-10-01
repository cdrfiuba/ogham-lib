#include <stddef.h>
#include <avr/io.h>
#include <util/delay.h>
#include "board.h"
#include "utils.h"


static uint8_t buttonDebounce;
static void (*buttonIsr)(void);
static void buttonPressed(void);


/**
 * Inicializa todo lo vinculado a la placa. Debe ser llamada siempre al comienzo
 * antes de usar cualquier elemento de la placa.
 **/
void initBoard(void)
{
    initLeds();
    initADC();
    initButton(NULL, 0);
}

/**
 * Inicializa los puertos de los leds.
 **/
void initLeds(void)
{
    uint8_t i;
    
    for (i=0; i<NUM_LEDS; i++)
    {
        configPin(&LED[i], 1, 0);  // pin como salida
        setPin(&LED[i]);  // iniciamos el LED apagado
    }
}

/**
 * Inicializa el botón.
 * @param isr Puntero a la función que será llamada cuando se presione el botón.
 * @param debounce Cantidad de milisegundos para filtrar los rebotes
 **/
void initButton(void (*isr)(void), uint8_t debounce)
{
    buttonIsr = isr;
    buttonDebounce = debounce;
    
    configPin(&BUTTON, 0, 1);  // pin como entrada con pullup interno
    if (isr != NULL)
        configExtInt(0, 2, buttonPressed);  // INT0, flanco descendente
}

/**
 * Inicializa el conversor analógico/digital para leer el LDR.
 **/
void initADC(void)
{
    IOPIN_t LDR = {&PORTC, &DDRC, &PINC, PIN0};
    
    // Configuro pin del LDR como entrada sin pullup
    configPin(&LDR, 0, 0);
    
    configADC(7);  // el prescaler depende de la velocidad del clock
    setADCChannel(0);
}

// callback
static void buttonPressed(void)
{
    // Filtramos los rebotes
    delay_ms(buttonDebounce);
    if (readButton() == 0)
    {
        buttonIsr();  // callback
    }
}

/**
 * Retorna el estado actual del botón.
 * @return
 *      @li 0: si el botón está presionado
 *      @li 1: si el botón no está presionado
 **/
uint8_t readButton(void)
{
    return readPin(&BUTTON);
}
