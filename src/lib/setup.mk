MMCU ?= atmega88
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
	LFUSE = 0xe4
	MMCU_N = 0
else
ifeq ($(MMCU), atmega88)
	TARGET_P = m88
	HFUSE = 0xdf
	# external clock, no divide by 8
	LFUSE = 0xf7
	MMCU_N = 1
else
	$(error Target no soportado)
endif
endif

CDEFINES = -DMMCU=$(MMCU_N) -DF_CPU=$(F_CPU)

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