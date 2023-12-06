CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` -Wall -Iincludes -Wextra -g -O3
LDFLAGS = -lSDL2 -lSDL2_image -lm `pkg-config --libs gtk+-3.0`

SRC_DIR = .
OBJ_DIR = obj

SRC = $(wildcard $(SRC_DIR)/*.c)

OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

EXECUTABLE = ocr

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR):
	mkdir -p $@

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(EXECUTABLE)
	rm *.bmp
	rm cropped/*
	rm grid2
	rm grid

.PHONY: run
run: $(EXECUTABLE)
	./$(EXECUTABLE)


$(OBJ_DIR)/image.o: includes/image.h includes/threshold.h
$(OBJ_DIR)/image_op.o: includes/image_op.h includes/image.h
$(OBJ_DIR)/main.o: includes/image.h includes/image_op.h includes/hough.h  
$(OBJ_DIR)/threshold.o: includes/threshold.h
$(OBJ_DIR)/contrast.o: includes/contrast.h includes/image_op.h
$(OBJ_DIR)/Sobel.o: includes/Sobel.h
$(OBJ_DIR)/grid.o: includes/Sobel.h
$(OBJ_DIR)/hough.o: includes/image_op.h
$(OBJ_DIR)/rotation.o: includes/rotation.h includes/image_op.h
