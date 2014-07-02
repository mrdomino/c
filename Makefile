CFLAGS=-g -Wall -pedantic -std=c89 -ansi
PROGS=ds hs ht ms qs rb

all: $(PROGS)

hs: util.o
qs: util.o
ms: util.o

util.o: util.h

clean:
	@echo cleaning
	@rm -f $(PROGS:%=%{,.o}) util.o
