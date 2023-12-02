#ifndef SEGMENTATION_H
#define SEGMENTATION_H


#include <SDL2/SDL.h>
void detection(SDL_Surface *image, SDL_Surface *seg);

void segmentation(SDL_Surface *image, int *xpos, int* ypos);



#endif
