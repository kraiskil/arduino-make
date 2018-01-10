
#Setup your arduino
# MCU - the AVR version, as understood by gcc
# FREQ - the frequency the board uses, in Hz
# PRGRM - target specific options for avrdude

# Arduino UNO
MCU=atmega328p
FREQ=16000000
PRGRM=-c arduino -P/dev/ttyACM0

#NH duino UNO (clone off ebay)
MCU=atmega328p
FREQ=16000000
PRGRM=-c arduino -P/dev/ttyUSB0

CC=avr-gcc
CXX=avr-g++
CXXSRC=main.cpp

CSRC=uart_stdout.c

COBJS=$(subst .c,.o,${CSRC})
CXXOBJS=$(subst .cpp,.o,${CXXSRC})
OBJS= ${COBJS} ${CXXOBJS}

CFLAGS=-mmcu=$(MCU) -DF_CPU=$(FREQ) -O2 -Wall -Werror
CXXFLAGS=${CFLAGS}


binary.elf: ${OBJS}
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean flash
flash: binary.elf
	avrdude -v -p $(MCU) $(PRGRM) -U flash:w:$^:e
	#avrdude -v -p atmega328p -c usbasp -U flash:w:$^:e

clean:
	rm -rf binary.elf ${OBJS}

