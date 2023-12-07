#ifndef IMAGE_H
#define IMAGE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Surface *load_image(char *filename);
void save_image(SDL_Surface *image, const char *filename);
void resize_image_GTK(SDL_Surface* image, char* filename);


#endif
