#ifndef SOBEL_H
#define SOBEL_H

#include <SDL2/SDL.h>


double Convolution(SDL_Surface* image, double kernel[3][3], int row, int col);
void sobel(SDL_Surface* image);

#endif

