#ifndef THRESHOLD_H
#define THRESHOLD_H

#include <SDL2/SDL.h>
#include "image_op.h"

Uint8 calculateOtsuThreshold(SDL_Surface *image);

#endif
