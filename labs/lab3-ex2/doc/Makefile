# File:   Makefile
# Author: M. P. Hayes, UCECE
# Date:   11 Sep 2010
# Descr:  Makefile for ex2 docs

DEL = rm

all: file_dependencies.pdf module_dependencies.pdf makefile_dependencies.pdf build_dependencies.pdf callgraph.pdf

file_dependencies.pdf: files.d 
	../../../etc/graphdeps.py $< --out $@

module_dependencies.pdf: modules.d 
	../../../etc/graphdeps.py $< --modules --out $@

makefile_dependencies.pdf: ../Makefile
	../../../etc/graphdeps.py $< --out $@

build_dependencies.pdf: ../Makefile
	../../../etc/graphdeps.py $< --out $@ --showops

callgraph.pdf: callgraph.d
	../../../etc/graphdeps.py --calls --modules $< --out $@ --showops


files.d: ../Makefile
	(cd ..;	../../etc/makemake.py --relpath --files . . ../../drivers ../../drivers/avr ../../utils ../../extra --exclude system.h > doc/files.d)


modules.d: ../Makefile
	(cd ..;	../../etc/makemake.py --relpath --modules . . ../../drivers ../../drivers/avr ../../utils ../../extra --exclude system > doc/modules.d)


callgraph.d: ../Makefile
	(cd ..;	../../etc/makemake.py --cc="avr-gcc" --cflags="-Os -mmcu=atmega32u2" --relpath --calls . . ../../drivers ../../drivers/avr ../../utils ../../extra --exclude system.h > doc/callgraph.d)


# Clean: delete derived files.
.PHONY: clean
clean: 
	-$(DEL) *.d *.pdf

