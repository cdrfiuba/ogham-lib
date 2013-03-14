#ifndef _BITMANIP_H_
#define _BITMANIP_H_

/**
 * Macros para manipulación de bits.
 **/


// Macros

#define true                    1
#define false                   0
#define nop()                   asm volatile("nop")

#define setBit(Byte,Bit)        ((Byte) |= (1<<(Bit)))
#define clearBit(Byte,Bit)      ((Byte) &= (~(1<<(Bit))))
#define isBitSet(Byte,Bit)      (((Byte) & (1<<(Bit))) ? true : false)
#define isBitClear(Byte,Bit)    (((Byte) & (1<<(Bit))) ? false : true)


#endif
