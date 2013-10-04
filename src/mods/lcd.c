/*
 * Biblioteca para el manejo de un display LCD.
 * Plataforma: Atmega8, Atmega88, Atmega48, Atmega168.
 */

#include <avr/io.h>
#include "lcd.h"
#include "bitmanip.h"
#include "pin.h"
#include "delay.h"

static IOPIN_t databusLCD;
static IOPIN_t enPinLCD;
static IOPIN_t rwPinLCD;
static IOPIN_t rsPinLCD;

static inline void writeByteLCD(uint8_t data);
static inline void writeNibbleLCD(uint8_t nib);


/**
 * Configura el conversor analógico/digital.
 * @param databus Puerto a donde están conectadas las líneas de datos del
 *                display. El campo IOPIN_t.bit debe ser 0 si las líneas que se
 *                usan corresponden al nibble bajo del puerto o 4 si es el
 *                nibble alto.
 *        enPin Pin de ENABLE
 *        rwPin Pin de READ/WRITE
 *        rsPin Pin de REGISTER SELECT
 * 
 * Pinout del display:
 *  01 - Ground (Vss)
 *  02 - 5V Power (Vcc)
 *  03 - Contrast (Vo)
 *  04 - Register Select (RS)
 *  05 - Read/Write_L (R/W_L)
 *  06 - Enable (E)
 *  07 - Data 0 (DB0)
 *  08 - Data 1 (DB1)
 *  09 - Data 2 (DB2)
 *  10 - Data 3 (DB3)
 *  11 - Data 4 (DB4)
 *  12 - Data 5 (DB5)
 *  13 - Data 6 (DB6)
 *  14 - Data 7 (DB7)
 *  15 - Backlight 5V Power (use 10 ohm resistor)
 *  16 - Ground (Vss)
 *
 * El pinout en el micro es el siguiente:
 *  enPin - Enable (E)
 *  rwPin - Read/Write_L (R/W_L)
 *  rsPin - Register Select (RS)
 *  dataBus.3 - Data 7 (DB7)
 *  dataBus.2 - Data 6 (DB6)
 *  dataBus.1 - Data 5 (DB5)
 *  dataBus.0 - Data 4 (DB4)
 * 
 * @see IOPIN_t
 */
void configLCD(const IOPIN_t *databus, const IOPIN_t *enPin, const IOPIN_t *rwPin, const IOPIN_t *rsPin)
{
    uint8_t i;
    
    // guardamos los punteros a los pines
    databusLCD = *databus;
    enPinLCD = *enPin;
    rwPinLCD = *rwPin;
    rsPinLCD = *rsPin;
    
    // configuramos el modo de los pines
    configPin(enPin, 1, 1);
    configPin(rwPin, 1, 1);
    configPin(rsPin, 1, 1);
    if (databus->bit > 3)  // usamos el nibble alto
    {
        for(i=4; i<8; i++)
        {
            databusLCD.bit = i;
            configPin(&databusLCD, 1, 1);
        }
        databusLCD.bit = 4;
    }
    else  // usamos el nibble bajo
    {
        for(i=0; i<4; i++)
        {
            databusLCD.bit = i;
            configPin(&databusLCD, 1, 1);
        }
        databusLCD.bit = 0;
    }
    
    delay_ms(15);
    
    // Function set: 4-bit mode, 2 lines, 5x8 dots
    sendCommandLCD(0x28);
    sendCommandLCD(0x28);
    sendCommandLCD(0x28);
    
    sendCommandLCD(0x0C);
    sendCommandLCD(0x01);
    sendCommandLCD(0x06);
}

/**
 * Imprime una cadena de texto por el LCD. La cadena debe ser de longitud menor
 * a 255 bytes y debe finalizar con ASCII cero.
 * @param buf puntero a la cadena de texto
 * @return la cantidad de caracteres impresos
 **/
uint8_t printLCD(char *buf)
{
    uint8_t i;
    
    for (i=0; i<256 && buf[i] != 0; i++)
        sendDataLCD(buf[i]);
    
    return i;
}

/**
 * Enviar un byte de comando al LCD.
 * @param cmd comando a enviar
 * @see configLCD()
 **/
void sendCommandLCD(uint8_t cmd)
{
    // clear register select
    clearPin(&enPinLCD);
    
    // write
    writeByteLCD(cmd);
    
    // wait
    delay_ms(1);
}

/**
 * Enviar un byte de dato al LCD.
 * @param data dato a enviar
 * @see configLCD()
 **/
void sendDataLCD(uint8_t data)
{
    // set register select
    setPin(&rsPinLCD);
    
    // write
    writeByteLCD(data);
    
    // wait
    delay_ms(1);
}

/**
 * Escribe un byte en el LCD.
 * @param b byte a escribir
 **/
void writeByteLCD(uint8_t data)
{
    // escribimos el nibble alto
    writeNibbleLCD(data >> 4);
    
    // escribimos el nibble bajo
    writeNibbleLCD(data);
}

/**
 * Escribe un nibble en el puerto del LCD. Usará el nibble bajo o alto de
 * acuerdo a cómo haya sido configurado.
 * @param nib se utilizan los 4 bits bajos del byte
 **/
void writeNibbleLCD(uint8_t nib)
{
    // set write mode
    clearPin(&rwPinLCD);
    
    // set enable
    setPin(&enPinLCD);
    
    // enmascaramos el nibble
    nib = nib & 0x0F;
    
    // preparamos el nibble que vamos a usar
    nib = nib << databusLCD.bit;
    
    // realizamos un proceso de enmascaramiento para no afectar el otro nibble
    // del puerto
    
    // ponemos en cero el nibble que vamos a cambiar
    *(databusLCD.port) &= ~(0x0F << databusLCD.bit);
        
    // escribimos el nibble sobre el puerto enmascarado
    *(databusLCD.port) |= nib;
    
    // clear enable
    clearPin(&enPinLCD);
}
