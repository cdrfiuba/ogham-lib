MMCU ?= atmega88
CLOCK_SOURCE ?= clock_int
F_CPU ?= 18432000UL

CC = avr-gcc
CFLAGS ?=
CFLAGS += -O2 -g -Wall
CFLAGS += -fdata-sections -ffunction-sections 
CFLAGS += -Wl,-gc-sections,-print-gc-sections
SRC ?= $(shell ls *.c)
SRC += $(shell ls $(OGHAMLIBPATH)/board/*.c)
SRC += $(shell ls $(OGHAMLIBPATH)/lib/*.c)
SRC += $(shell ls $(OGHAMLIBPATH)/mods/*.c)
INC ?= 
INC += -I$(OGHAMLIBPATH)/

ifeq ($(MMCU), atmega8)
	TARGET_P = m8
	HFUSE = 0xd9
	ifeq ($(CLOCK_SOURCE), clock_int_1m)
		LFUSE = 0xe1
	else ifeq ($(CLOCK_SOURCE), clock_int_8m)
		LFUSE = 0xe4
	else ifeq ($(CLOCK_SOURCE), clock_ext)
		HFUSE = 0xc9
		LFUSE = 0xef
	else
		$(error Fuente de reloj no válida.)
	endif
else ifeq ($(MMCU), atmega88)
	TARGET_P = m88
	HFUSE = 0xdf
	ifeq ($(CLOCK_SOURCE), clock_int_1m)
		LFUSE = 0x62
	else ifeq ($(CLOCK_SOURCE), clock_int_8m)
		LFUSE = 0xe2
	else ifeq ($(CLOCK_SOURCE), clock_ext)
		LFUSE = 0xf7
	else
		$(error Fuente de reloj no válida.)
	endif
else ifeq ($(MMCU), atmega168)
	TARGET_P = m168
	HFUSE = 0xdf
	ifeq ($(CLOCK_SOURCE), clock_int_1m)
		LFUSE = 0x62
	else ifeq ($(CLOCK_SOURCE), clock_int_8m)
		LFUSE = 0xd2
	else ifeq ($(CLOCK_SOURCE), clock_ext)
		LFUSE = 0xf7
	else
		$(error Fuente de reloj no válida.)
	endif
else ifeq ($(MMCU), atmega328)
	TARGET_P = m328
	HFUSE = 0xf9
	ifeq ($(CLOCK_SOURCE), clock_int_1m)
		LFUSE = 0x62
	else ifeq ($(CLOCK_SOURCE), clock_int_8m)
		LFUSE = 0xd2
	else ifeq ($(CLOCK_SOURCE), clock_ext)
		LFUSE = 0xf7
	else
		$(error Fuente de reloj no válida.)
	endif
else
	$(error Target no soportado)
endif

CDEFINES = -DF_CPU=$(F_CPU)

# Reglas
########

# Reglas para compilar y generar el binario para subir al target
all: hex

elf: $(SRC)
	$(CC) $(CDEFINES) -mmcu=$(MMCU) $(CFLAGS) $(INC) -o $(TARGET).elf $(SRC) 
	avr-objdump -h -S $(TARGET).elf > $(TARGET).lst
	avr-size -d --format=avr --mcu=$(MMCU) $(TARGET).elf

hex: elf
	avr-objcopy -j .text -j .data -O ihex $(TARGET).elf $(TARGET).hex


# Reglas para programar el target
program: hex
	avrdude -c usbtiny -p $(TARGET_P) -U f:w:$(TARGET).hex

program_dw: elf
	avarice -w -j usb --erase --program --file $(TARGET).elf

fuse:
	avrdude -c usbtiny -p $(TARGET_P) -U lfuse:w:$(LFUSE):m -U hfuse:w:$(HFUSE):m

clean:
	rm -rf $(TARGET).hex $(TARGET).elf $(TARGET).lst
