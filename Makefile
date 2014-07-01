CFLAGS=-g -Wall -pedantic -std=c89 -ansi

all: ht rb
ht: ht.o
rb: rb.o
clean:
	-rm ht{,.o} rb{,.o}
