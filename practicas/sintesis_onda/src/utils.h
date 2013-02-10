#ifndef _UTILS_H_
#define _UTILS_H_

#define true                    1
#define false                   0
#define Nop()                   asm volatile("nop")

#define SetBit(Byte,Bit)        (Byte |= (1<<Bit))
#define ClearBit(Byte,Bit)      (Byte &= (~(1<<Bit)))
#define IsBitSet(Byte,Bit)      ( (Byte & (1<<Bit)) ? true : false )
#define IsBitClear(Byte,Bit)    ( (Byte & (1<<Bit)) ? false : true )

#define concat(a,b)             a ## b
#define def_port_reg(name)      concat(PORT,name)
#define def_pin_reg(name)       concat(PIN,name)
#define def_ddr_reg(name)       concat(DDR,name)

#endif
