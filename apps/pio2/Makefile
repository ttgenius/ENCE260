# File:   Makefile
# Author: M. P. Hayes, UCECE
# Date:   12 Sep 2010
# Descr:  Makefile for pio2

# Definitions.
CC = avr-gcc
CFLAGS = -mmcu=atmega32u2 -Os -Wall -Wstrict-prototypes -Wextra -g -I../../drivers/avr
OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm


# Default target.
all: pio2.out


# Compile: create object files from C source files.
pio2.o: pio2.c ../../drivers/avr/pio.h ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

pio.o: ../../drivers/avr/pio.c ../../drivers/avr/pio.h ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

system.o: ../../drivers/avr/system.c ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@




# Link: create output file (executable) from object files.
pio2.out: pio2.o pio.o system.o
	$(CC) $(CFLAGS) $^ -o $@ -lm
	$(SIZE) $@


# Create hex file for programming from executable file.
pio2.hex: pio2.out
	$(OBJCOPY) -O ihex pio2.out pio2.hex


# Target: clean project.
.PHONY: clean
clean: 
	-$(DEL) *.o *.out *.hex


# Target: program project.
.PHONY: program
program: pio2.hex
	dfu-programmer atmega32u2 erase; dfu-programmer atmega32u2 flash pio2.hex; dfu-programmer atmega32u2 start


