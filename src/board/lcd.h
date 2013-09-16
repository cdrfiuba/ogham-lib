#ifndef _LCD_H_
#define _LCD_H_

/**
 * @file lcd.h
 * Cabecera para el manejo de un display LCD.
 * Plataforma: Atmega8, Atmega88, Atmega48, Atmega168.
 **/

#include "pin.h"

void configLCD(const IOPIN_t *databus, const IOPIN_t *enPin, const IOPIN_t *rwPin, const IOPIN_t *rsPin);
void sendCommandLCD(uint8_t cmd);
void sendDataLCD(uint8_t data);


#endif
