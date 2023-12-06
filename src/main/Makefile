CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` -Wall -Iincludes -Wextra -g
LDFLAGS = -lSDL2 -lSDL2_image -lm

SRC_DIRS = image Interface Rotation solver
OBJ_DIR = obj

SRC = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))

OBJ := $(addprefix $(OBJ_DIR)/,$(notdir $(SRC:.c=.o)))

EXECUTABLE = ocr

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR):
	mkdir -p $@

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(EXECUTABLE)
	rm *.bmp
	rm cropped/*

.PHONY: run
run: $(EXECUTABLE)
	./$(EXECUTABLE)

$(OBJ_DIR)/image.o: image/includes/image.h image/includes/threshold.h
$(OBJ_DIR)/image_op.o: image/includes/image_op.h image/includes/image.h
$(OBJ_DIR)/main.o: image/includes/image.h image/includes/image_op.h
$(OBJ_DIR)/threshold.o: image/includes/threshold.h
$(OBJ_DIR)/contrast.o: image/includes/contrast.h image/includes/image_op.h
$(OBJ_DIR)/Sobel.o: image/includes/Sobel.h
$(OBJ_DIR)/grid.o: image/includes/Sobel.h
$(OBJ_DIR)/hough.o: image/includes/image_op.h
$(OBJ_DIR)/rotation.o: image/includes/rotation.h image/includes/image_op.h
$(OBJ_DIR)/segmentation.o: image/includes/image_op.h image/includes/image.h
