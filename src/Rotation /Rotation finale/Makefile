# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm

all: rotation2

SRC = rotation2.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

rotation: rotation2.o

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END
