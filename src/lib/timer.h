#ifndef _TIMER_H_
#define _TIMER_H_

/**
 * @file timer.h
 * Cabecera para el manejo del timer en Atmega88.
 **/

// Tipos

typedef struct
{
    uint8_t prescaler;
    uint8_t mode;
    void (*isr)(void);   // interrupt service routine
} Timer0_t;

Timer0_t Timer0;

// Funciones públicas
#ifdef __cplusplus
extern "C"
{
#endif

void configTimer0(uint8_t prescaler, void (*isr)(void));
void startTimer0(void);
void readTimer0(void);
void stopTimer0(void);

#ifdef __cplusplus
}
#endif

/*

f_clk = 18.432.000 Hz
t_clk = 54.3 ns

timer minimal resolution:
res_timer = t_clk * 256 * prescaler


t_timer = res_timer * 256


prescaler       res             max count
1               13.9 us         3.56 ms
8               111  us         28.4 ms
32              444  us         114  ms
64              889  us         228  ms
128             1.78 ms         455  ms
256             3.56 ms         910  ms
1024            14.2 ms         3.64 s

sensor de ultrasonido
suponemos máxima distancia útil: 2m
v_sonido = 340 m/s = 340 mm/ms = 0.34 mm/us
distancia = v_sonido * t / 2

prescaler       res             res dist       max count        max dist
1               13.9 us         2.36 mm        3.56 ms          60.5 cm
8               111  us         18.9 mm        28.4 ms          4.84 m
32              444  us         75.5 mm        114  ms
64              889  us         151 mm         228  ms
128             1.78 ms                     455  ms
256             3.56 ms                     910  ms
1024            14.2 ms                     3.64 s


*/



#endif
