# Micros soportados:
# atmega8, atmega88, atmega88p, atmega168, atmega168p, atmega328, atmega328p
MMCU ?= atmega88

# Fuente del reloj:
# clock_int_1m : reloj interno de 1 MHz
# clock_int_8m : reloj interno de 8 MHz
# clock_ext : reloj externo
CLOCK_SOURCE ?= clock_ext

# Frecuencia del reloj en hertz
F_CPU ?= 18432000UL
