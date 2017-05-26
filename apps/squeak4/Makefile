# File:   Makefile
# Author: M. P. Hayes, UCECE
# Date:   12 Sep 2010
# Descr:  Makefile for squeak4

# Definitions.
CC = avr-gcc
CFLAGS = -mmcu=atmega32u2 -Os -Wall -Wstrict-prototypes -Wextra -g -I../../drivers/avr -I../../extra -I../../drivers -I../../fonts -I../../utils
OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm


# Default target.
all: squeak4.out


# Compile: create object files from C source files.
squeak4.o: squeak4.c ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/avr/timer.h ../../drivers/button.h ../../drivers/display.h ../../drivers/led.h ../../extra/mmelody.h ../../extra/ticker.h ../../extra/tweeter.h ../../fonts/font3x5_1.h ../../utils/font.h ../../utils/task.h ../../utils/tinygl.h
	$(CC) -c $(CFLAGS) $< -o $@

pio.o: ../../drivers/avr/pio.c ../../drivers/avr/pio.h ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

system.o: ../../drivers/avr/system.c ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

timer.o: ../../drivers/avr/timer.c ../../drivers/avr/system.h ../../drivers/avr/timer.h
	$(CC) -c $(CFLAGS) $< -o $@

button.o: ../../drivers/button.c ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/button.h
	$(CC) -c $(CFLAGS) $< -o $@

display.o: ../../drivers/display.c ../../drivers/avr/system.h ../../drivers/display.h ../../drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

led.o: ../../drivers/led.c ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/led.h
	$(CC) -c $(CFLAGS) $< -o $@

ledmat.o: ../../drivers/ledmat.c ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

mmelody.o: ../../extra/mmelody.c ../../drivers/avr/system.h ../../extra/mmelody.h
	$(CC) -c $(CFLAGS) $< -o $@

ticker.o: ../../extra/ticker.c
	$(CC) -c $(CFLAGS) $< -o $@

tweeter.o: ../../extra/tweeter.c ../../drivers/avr/system.h ../../extra/ticker.h ../../extra/tweeter.h
	$(CC) -c $(CFLAGS) $< -o $@

font.o: ../../utils/font.c ../../drivers/avr/system.h ../../utils/font.h
	$(CC) -c $(CFLAGS) $< -o $@

task.o: ../../utils/task.c ../../drivers/avr/system.h ../../drivers/avr/timer.h ../../utils/task.h
	$(CC) -c $(CFLAGS) $< -o $@

tinygl.o: ../../utils/tinygl.c ../../drivers/avr/system.h ../../drivers/display.h ../../utils/font.h ../../utils/tinygl.h
	$(CC) -c $(CFLAGS) $< -o $@




# Link: create output file (executable) from object files.
squeak4.out: squeak4.o pio.o system.o timer.o button.o display.o led.o ledmat.o mmelody.o ticker.o tweeter.o font.o task.o tinygl.o
	$(CC) $(CFLAGS) $^ -o $@ -lm
	$(SIZE) $@


# Create hex file for programming from executable file.
squeak4.hex: squeak4.out
	$(OBJCOPY) -O ihex squeak4.out squeak4.hex


# Target: clean project.
.PHONY: clean
clean: 
	-$(DEL) *.o *.out *.hex


# Target: program project.
.PHONY: program
program: squeak4.hex
	dfu-programmer atmega32u2 erase; dfu-programmer atmega32u2 flash squeak4.hex; dfu-programmer atmega32u2 start


