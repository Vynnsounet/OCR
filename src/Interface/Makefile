# Makefile

CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` -Wall -O3 -Iincludes
LDLIBS = `pkg-config --libs gtk+-3.0` 
LDFLAGS = -lSDL2 -lSDL2_image -lm

EXE = interface 

all: $(EXE)

$(foreach f, $(EXE), $(eval $(f):))

.PHONY: clean

clean:
	${RM} $(EXE)

# END
