CFLAGS=-g -Wall -pedantic -std=c89 -ansi

all: ht rb hs
ht: ht.o
rb: rb.o
hs: hs.o
clean:
	-rm ht{,.o} rb{,.o} hs{,.o}
