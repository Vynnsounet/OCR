CC=gcc
CFLAGS=-Wall -Wextra

all: obj
	gcc -o tests/test matlib.o testfile.o

test: all
	./tests/test
	make clean

obj:
	gcc -c matlib.c
	gcc -c tests/testfile.c

clean: 
	rm matlib.o testfile.o
	rm tests/test
