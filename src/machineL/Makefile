C_SOURCES = $(wildcard matrix/*.c neural/*.c util/*.c *.c)
HEADERS = $(wildcard matrix/*.h neural/*.h util/*.h *.h)
OBJ = ${C_SOURCES:.c=.o}
CFLAGS = -fsanitize=address -lm -lSDL2 -lSDL2_image -g

MAIN = main
CC = /usr/bin/gcc
LINKER = /usr/bin/ld

all: ${OBJ}
	unzip data/mnist-in-csv.zip -d data
	${CC} ${CFLAGS} $^ -o main

run: ${MAIN}
	./${MAIN}

# Generic rules
%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm -f matrix/*.o *.o neural/*.o util/*.o ${MAIN}
	rm -f data/*.csv
