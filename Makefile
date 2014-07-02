CFLAGS=-g -Wall -pedantic -std=c89 -ansi

all: ht rb hs qs
ht: ht.o
rb: rb.o
hs: hs.o util.o
qs: qs.o util.o
ms: ms.o util.o
util.o: util.h
clean:
	-rm ht{,.o} rb{,.o} hs{,.o} qs{,.o} util.o
