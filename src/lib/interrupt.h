#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

/**
 * @file interrupt.h
 * Cabecera para el manejo de interrupciones en Atmega8 y Atmega88.
 **/


// Interrupciones
#ifdef __cplusplus
extern "C"
{
#endif

void configExtInt(uint8_t num, uint8_t sense, void (*isr)(void));

#ifdef __cplusplus
}
#endif

#endif
