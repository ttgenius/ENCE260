# File:   Makefile
# Author: R. Lobb and J. Chen
# Date:   Aug 2014
# Descr:  Makefile for processstudent


# Definitions.
CFLAGS = -g -std=c99 -Wall -Werror


# Main target: create executable by linking the object files
processstudents: processstudents.o student.o studentlist.o
	gcc processstudents.o student.o studentlist.o -o processstudents


# Intermediate targets: create object files from C source codes.
processstudents.o: processstudents.c student.h studentlist.h
	gcc $(CFLAGS) -c processstudents.c

student.o: student.c student.h
	gcc $(CFLAGS) -c student.c

studentlist.o: studentlist.c student.h studentlist.h
	gcc $(CFLAGS) -c studentlist.c


# Clean up target (used with the command "make clean")
clean:
	rm *.o
