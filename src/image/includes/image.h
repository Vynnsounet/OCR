#ifndef IMAGE_H
#define IMAGE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Surface *load_image(char *filename);
void save_image(SDL_Surface *image, const char *filename);

#endif
