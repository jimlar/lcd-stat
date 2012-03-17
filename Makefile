#
# Makefile for the LCD project
#
# Jimmy Larson 1997-03-31
#

CC = gcc
CFLAGS = -g -O
OBJS = lcd.o stat.o display.o log.o lcdd.o


all:	lcdd

lcdd:	$(OBJS)
	$(CC) -o lcdd $(OBJS)

clean:	
	rm -f lcdd *.o *~ \#*

# dependencies
lcd.o:		lcd.h
stat.o:		stat.h log.h
dislplay.o:	display.h stat.h lcd.h
lcdd.o:		display.h lcd.h
log.o:          log.h

