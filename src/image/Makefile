CC = gcc
CFLAGS = -Wall -Iincludes -Wextra
LDFLAGS = -lSDL2 -lSDL2_image -lm

# Source files and object files
SRC_DIR = .
OBJ_DIR = obj

# List of source files
SRC = $(wildcard $(SRC_DIR)/*.c)

# Generate a list of object files from source files
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

# Executable name
EXECUTABLE = my_program

# Define your targets and their dependencies
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

# Define any additional dependencies for your object files here

# Example: If image.c depends on image.h and threshold.h
$(OBJ_DIR)/image.o: includes/image.h includes/threshold.h
$(OBJ_DIR)/image_op.o: includes/image_op.h includes/image.h
$(OBJ_DIR)/main.o: includes/image.h includes/image_op.h
$(OBJ_DIR)/threshold.o: includes/threshold.h

# Define dependencies for contrast.c
$(OBJ_DIR)/contrast.o: includes/contrast.h includes/image_op.h
