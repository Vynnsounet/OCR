#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <SDL2/SDL.h>

void labelConnectedComponents(SDL_Surface* image, int label, int x, int y);

void keepLargestComponent(SDL_Surface* image);

#endif
