#ifndef HOUGH_H
#define HOUGH_H

#include <SDL2/SDL.h>

void HoughTransform(SDL_Surface *image);
//void houghTransform(SDL_Surface *image);
void drawLine(int x0, int y0, int x1, int y1, SDL_Surface * image);
int HoughVerif(SDL_Surface* image);
int automaticrotation(SDL_Surface * image);


#endif
