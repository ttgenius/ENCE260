# File:   Makefile
# Author: M. P. Hayes, UCECE
# Date:   12 Sep 2010
# Descr:  Makefile for lab1-ex2

# Definitions.
CC = avr-gcc
CFLAGS = -mmcu=atmega32u2 -Os -Wall -Wstrict-prototypes -Wextra -g -I../../drivers/avr
OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm


# Default target.
all: lab1-ex2.out


# Compile: create object files from C source files.
lab1-ex2.o: lab1-ex2.c ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

system.o: ../../drivers/avr/system.c ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@



# Link: create ELF output file from object files.
lab1-ex2.out: lab1-ex2.o system.o
	$(CC) $(CFLAGS) $^ -o $@ -lm
	$(SIZE) $@


# Target: clean project.
.PHONY: clean
clean: 
	-$(DEL) *.o *.out *.hex


# Target: program project.
.PHONY: program
program: lab1-ex2.out
	$(OBJCOPY) -O ihex lab1-ex2.out lab1-ex2.hex
	dfu-programmer atmega32u2 erase; dfu-programmer atmega32u2 flash lab1-ex2.hex; dfu-programmer atmega32u2 start


