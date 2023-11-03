#ifndef CONTRAST_H
#define CONTRAST_H


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "image_op.h"


void reduceContrast(SDL_Surface *surface, double factor);

#endif
