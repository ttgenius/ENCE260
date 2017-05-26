# File:   Makefile
# Author: M. P. Hayes, UCECE
# Date:   12 Sep 2010
# Descr:  Makefile for squeak1

# Definitions.
CC = avr-gcc
CFLAGS = -mmcu=atmega32u2 -Os -Wall -Wstrict-prototypes -Wextra -g -I../../drivers/avr -I../../extra -I../../drivers -I../../utils
OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm


# Default target.
all: squeak1.out


# Compile: create object files from C source files.
squeak1.o: squeak1.c ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/avr/timer.h ../../drivers/led.h ../../extra/mmelody.h ../../extra/ticker.h ../../extra/tweeter.h ../../utils/task.h
	$(CC) -c $(CFLAGS) $< -o $@

pio.o: ../../drivers/avr/pio.c ../../drivers/avr/pio.h ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

system.o: ../../drivers/avr/system.c ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

timer.o: ../../drivers/avr/timer.c ../../drivers/avr/system.h ../../drivers/avr/timer.h
	$(CC) -c $(CFLAGS) $< -o $@

led.o: ../../drivers/led.c ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/led.h
	$(CC) -c $(CFLAGS) $< -o $@

mmelody.o: ../../extra/mmelody.c ../../drivers/avr/system.h ../../extra/mmelody.h
	$(CC) -c $(CFLAGS) $< -o $@

ticker.o: ../../extra/ticker.c
	$(CC) -c $(CFLAGS) $< -o $@

tweeter.o: ../../extra/tweeter.c ../../drivers/avr/system.h ../../extra/ticker.h ../../extra/tweeter.h
	$(CC) -c $(CFLAGS) $< -o $@

task.o: ../../utils/task.c ../../drivers/avr/system.h ../../drivers/avr/timer.h ../../utils/task.h
	$(CC) -c $(CFLAGS) $< -o $@




# Link: create output file (executable) from object files.
squeak1.out: squeak1.o pio.o system.o timer.o led.o mmelody.o ticker.o tweeter.o task.o
	$(CC) $(CFLAGS) $^ -o $@ -lm
	$(SIZE) $@


# Create hex file for programming from executable file.
squeak1.hex: squeak1.out
	$(OBJCOPY) -O ihex squeak1.out squeak1.hex


# Target: clean project.
.PHONY: clean
clean: 
	-$(DEL) *.o *.out *.hex


# Target: program project.
.PHONY: program
program: squeak1.hex
	dfu-programmer atmega32u2 erase; dfu-programmer atmega32u2 flash squeak1.hex; dfu-programmer atmega32u2 start


