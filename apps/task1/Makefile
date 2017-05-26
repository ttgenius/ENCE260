# File:   Makefile
# Author: M. P. Hayes, UCECE
# Date:   12 Sep 2010
# Descr:  Makefile for task1

# Definitions.
CC = avr-gcc
CFLAGS = -mmcu=atmega32u2 -Os -Wall -Wstrict-prototypes -Wextra -g -I../../drivers -I../../drivers/avr -I../../utils
OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm


# Default target.
all: task1.out


# Compile: create object files from C source files.
task1.o: task1.c ../../drivers/avr/system.h ../../drivers/avr/timer.h ../../drivers/led.h ../../utils/task.h
	$(CC) -c $(CFLAGS) $< -o $@

pio.o: ../../drivers/avr/pio.c ../../drivers/avr/pio.h ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

system.o: ../../drivers/avr/system.c ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

timer.o: ../../drivers/avr/timer.c ../../drivers/avr/system.h ../../drivers/avr/timer.h
	$(CC) -c $(CFLAGS) $< -o $@

led.o: ../../drivers/led.c ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/led.h
	$(CC) -c $(CFLAGS) $< -o $@

task.o: ../../utils/task.c ../../drivers/avr/system.h ../../drivers/avr/timer.h ../../utils/task.h
	$(CC) -c $(CFLAGS) $< -o $@




# Link: create output file (executable) from object files.
task1.out: task1.o pio.o system.o timer.o led.o task.o
	$(CC) $(CFLAGS) $^ -o $@ -lm
	$(SIZE) $@


# Create hex file for programming from executable file.
task1.hex: task1.out
	$(OBJCOPY) -O ihex task1.out task1.hex


# Target: clean project.
.PHONY: clean
clean: 
	-$(DEL) *.o *.out *.hex


# Target: program project.
.PHONY: program
program: task1.hex
	dfu-programmer atmega32u2 erase; dfu-programmer atmega32u2 flash task1.hex; dfu-programmer atmega32u2 start


