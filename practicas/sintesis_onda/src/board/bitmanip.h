#ifndef _BITMANIP_H_
#define _BITMANIP_H_

/**
 * @file bitmanip.h
 * Macros para manipulación de bits.
 **/


// Macros

/**
 * Constante true
 */
#define true                    1

/**
 * Constante false
 */
#define false                   0

/**
 * Instrucción vacía. Sirve para insertar una demora de exactamente un ciclo de
 * reloj.
 */
#define nop()                   asm volatile("nop")

/**
 * Pone un bit en uno dentro de un byte.
 * @param Byte palabra de 8 bits
 * @param Bit posición del bit a encender. Rango de 0 a 7.
 */
#define setBit(Byte,Bit)        ((Byte) |= (1<<(Bit)))

/**
 * Pone un bit en cero dentro de un byte.
 * @param Byte palabra de 8 bits
 * @param Bit posición del bit a apagar. Rango de 0 a 7.
 */
#define clearBit(Byte,Bit)      ((Byte) &= (~(1<<(Bit))))

/**
 * Indica si un bit está encendido.
 * @param Byte palabra de 8 bits
 * @param Bit posición del bit. Rango de 0 a 7.
 * @return
 *      @li true: si está encendido
 *      @li false: si está apagado
 */
#define isBitSet(Byte,Bit)      (((Byte) & (1<<(Bit))) ? true : false)

/**
 * Indica si un bit está apagado.
 * @param Byte palabra de 8 bits
 * @param Bit posición del bit. Rango de 0 a 7.
 * @return
 *      @li true: si está apagado
 *      @li false: si está encendido
 */
#define isBitClear(Byte,Bit)    (((Byte) & (1<<(Bit))) ? false : true)


#endif
