CC=gcc
CFLAGS=-Wall -Wextra

all: obj
	gcc -o tests/test matlib.o testfile.o -g

test: all
	./tests/test
	make clean

obj:
	gcc -c matlib.c -g
	gcc -c tests/testfile.c -g

clean: 
	rm matlib.o testfile.o
	rm tests/test
