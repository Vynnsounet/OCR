CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11
LDFLAGS = -lSDL2 -lSDL2_image -lm

SRC = main.c
OBJ = $(SRC:.c=.o)
EXECUTABLE = grayscale_converter

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(OBJ) $(EXECUTABLE)
	rm -f *.bmp

.PHONY: all clean