#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <SDL2/SDL.h>

Uint32 get_pixel(SDL_Surface *image, int y, int x);
void put_pixel(SDL_Surface *image, int y, int x, Uint32 rgba);
void grayscale(SDL_Surface *image);
void binarize(SDL_Surface *image, Uint8 threshold);
void convolution(SDL_Surface *image, size_t k, double matrix[][5]);

#endif
