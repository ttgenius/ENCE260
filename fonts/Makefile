CC = gcc
#CFLAGS = -O2 -Wall -W -g
CFLAGS = -O0 -Wall -W -g

FONTDEFS = $(wildcard *.txt)
FONTS = $(FONTDEFS:.txt=.h) $(FONTDEFS:.txt=_r.h)


all: fontgen $(FONTS)


%.h: %.txt fontgen
	./fontgen $(@:.h=) < $^  > $@

%_r.h: %.txt fontgen
	./fontgen $(@:.h=) --rotate < $^  > $@


fontgen: fontgen.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm *.o fontgen
