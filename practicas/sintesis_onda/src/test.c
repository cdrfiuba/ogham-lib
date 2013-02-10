#include "utils.h"
//#include "comm.h"
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define Led1Init()  SetBit(DDRC, PIN3)
#define Led1On()    ClearBit(PORTC, PIN3)
#define Led1Off()   SetBit(PORTC, PIN3)
#define IsLed1On()    IsBitSet(PORTC, PIN3)
#define Led1Toggle()  {if ( IsLed1On() ) Led1Off(); else Led1On();}

#define PRESCALER_PWM_OFF	(~((1<<CS22) | (1<<CS21) | (1<<CS20)))
#define PRESCALER_PWM_ON	(0<<CS22)|(0<<CS21)|(1<<CS20)

// modo fast PWM timer 2 con top fijo, salida de PWM por pin OC2a
#define PWM_TCCR2A ( (1<<COM2A1) | (0<<COM2A0) | (1<<COM2B1) | (0<<COM2B0) | (1<<WGM21) | (1<<WGM20) )
#define PWM_TCCR2B ( (0<<WGM22) & PRESCALER_PWM_OFF )

// Interrupciones en comparaciones
#define TIMSK2_VALUE  ( (1<<TOIE2) | (0<<OCIE2A) | (0<<OCIE2B) )

#define PwmStart() (TCCR2B |= PRESCALER_PWM_ON)
#define PwmStop()  (TCCR2B &= PRESCALER_PWM_OFF)

//char sin_table[] PROGMEM = {128,134,140,147,153,160,166,172,178,184,189,195,200,205,210,215,219,223,227,231,234,238,240,243,245,248,249,251,252,253,254,255,255,255,255,255,254,253,253,252,251,249,248,247,245,244,242,241,239,238,236,235,233,232,231,230,229,228,227,226,225,225,225,225,224,225,225,225,226,226,227,228,229,230,231,233,234,236,237,239,240,242,243,245,246,247,249,250,251,252,253,254,254,255,255,255,255,254,254,253,252,250,248,247,244,242,239,236,233,229,225,221,217,212,208,203,197,192,186,181,175,169,163,156,150,144,137,131,124,118,111,105,99,92,86,80,74,69,63,58,52,47,43,38,34,30,26,22,19,16,13,11,8,7,5,3,2,1,1,0,0,0,0,1,1,2,3,4,5,6,8,9,10,12,13,15,16,18,19,21,22,24,25,26,27,28,29,29,30,30,30,31,30,30,30,30,29,28,27,26,25,24,23,22,20,19,17,16,14,13,11,10,8,7,6,4,3,2,2,1,0,0,0,0,0,1,2,3,4,6,7,10,12,15,17,21,24,28,32,36,40,45,50,55,60,66,71,77,83,89,95,102,108,115,121,127};
char sin_table[] PROGMEM = {128,131,134,137,140,143,146,149,152,156,159,162,165,168,171,174,176,179,182,185,188,191,193,196,199,201,204,206,209,211,213,216,218,220,222,224,226,228,230,232,234,235,237,239,240,242,243,244,246,247,248,249,250,251,251,252,253,253,254,254,254,255,255,255,255,255,255,255,254,254,253,253,252,252,251,250,249,248,247,246,245,244,242,241,239,238,236,235,233,231,229,227,225,223,221,219,217,215,212,210,207,205,202,200,197,195,192,189,186,184,181,178,175,172,169,166,163,160,157,154,151,148,145,142,138,135,132,129,126,123,120,117,113,110,107,104,101,98,95,92,89,86,83,80,77,74,71,69,66,63,60,58,55,53,50,48,45,43,40,38,36,34,32,30,28,26,24,22,20,19,17,16,14,13,11,10,9,8,7,6,5,4,3,3,2,2,1,1,0,0,0,0,0,0,0,1,1,1,2,2,3,4,4,5,6,7,8,9,11,12,13,15,16,18,20,21,23,25,27,29,31,33,35,37,39,42,44,46,49,51,54,56,59,62,64,67,70,73,76,79,81,84,87,90,93,96,99,103,106,109,112,115,118,121,124,127};

volatile uint16_t phase = 0;
volatile uint8_t phase_increment = 1;

void configurarTimer2(void) {
	// Configuracion del timer 2 para el PWM
	TCCR2A = PWM_TCCR2A;  
	TCCR2B = PWM_TCCR2B; 	

	//Habilitamos la interrupcion del Timer2 de comparacion
	TIMSK2 = TIMSK2_VALUE;
}
volatile unsigned int contador = 0;
// Timer 2 overflow interrupt
ISR(TIMER2_OVF_vect)
{
    // cargar valor de la tabla al OCR2A
    OCR2B = pgm_read_byte(&sin_table[phase]);
    contador++;
    /*if (contador==10000) {
    //if (phase < sizeof(sin_table)) phase++; else phase = 0;
        phase = (phase + phase_increment) % sizeof(sin_table);
        contador = 0;
    }*/
    if (phase > 256-phase_increment-1)
    {
        phase_increment++;
        if (phase_increment > 128) phase_increment = 1;
    }
}


void startup (void) {
    configurarTimer2();
    
    sei();  // habilitar interrupciones
    
    //Configuramos como salidas los pines de PWM
    SetBit(DDRB, PIN3);
    SetBit(DDRD, PIN3);
    
    Led1Init();
    Led1Off();
}

int main (void)
{
    startup();
    PwmStart();
    
    // Bucle infinito
    while(1)
    {
        Led1Toggle();  // parpadear el led para saber que esta vivo
        _delay_ms(250); // demora para que podamos apreciar el parpadeo
    }
}
