CC = gcc
CFLAGS = -Wall -Iincludes -Wextra
LDFLAGS = -lSDL2 -lSDL2_image -lm

SRC_DIR = .
OBJ_DIR = obj

SRC = $(wildcard $(SRC_DIR)/*.c)

OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

EXECUTABLE = my_program

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

.PHONY: run
run: $(EXECUTABLE)
	./$(EXECUTABLE)


$(OBJ_DIR)/image.o: includes/image.h includes/threshold.h
$(OBJ_DIR)/image_op.o: includes/image_op.h includes/image.h
$(OBJ_DIR)/main.o: includes/image.h includes/image_op.h
$(OBJ_DIR)/threshold.o: includes/threshold.h
$(OBJ_DIR)/contrast.o: includes/contrast.h includes/image_op.h
$(OBJ_DIR)/Sobel.o: includes/Sobel.h
$(OBJ_DIR)/grid.o: includes/Sobel.h
