CFLAGS=-Wall -pedantic -std=c89 -ansi
PROGS=ds hs ht ms qs rb

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

hs qs ms: util.o

util.o: util.h

clean:
	@echo cleaning
	@rm -f $(PROGS:%=%{,.o}) util.o

grind: all
	@for p in $(PROGS); do echo valgrind ./$$p; valgrind -q ./$$p >/dev/null ; done
