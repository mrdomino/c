CFLAGS=-Wall -pedantic -std=c99 -ansi
PROGS=ds gr hs ht ms qs rb

CFLAGS+=-g

#CFLAGS+=-Os -DNDEBUG
#LDFLAGS=-s


all: options $(PROGS)

options:
	@echo build options:
	@echo "CFLAGS  = $(CFLAGS)"
	@echo "LDFLAGS = $(LDFLAGS)"
	@echo "CC      = $(CC)"

.for p in $(PROGS)
$p: $p.o
	@echo CC -o $@
	@$(CC) $(LDFLAGS) $(.ALLSRC) -o $@
.endfor

.SUFFIXES: .c
.c.o:
	@echo CC $<
	@$(CC) -c $(CFLAGS) $<

gr: queue.o
hs qs ms: util.o

util.o: util.h

clean:
	@echo cleaning
	@rm -f $(PROGS:%=%{,.o}) util.o queue.o

grind: all
	@for p in $(PROGS); do echo valgrind ./$$p; valgrind -q ./$$p >/dev/null ; done
