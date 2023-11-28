#include <SDL2/SDL.h>
#include "includes/image_op.h"
#include "includes/hough.h"




void detection(SDL_Surface *image)
{
    int count = 0;
    int coordx[9];
    for (int y = 0; y < image->h; y++) {
        for (int x = 0; x < image->w; x++) {
            Uint8 red, green, blue, alpha;
            Uint32 pixel = get_pixel(image, y, x);
            SDL_GetRGBA(pixel, image->format, &red, &green, &blue, &alpha);
            if(red==255)
            {
                if
            }
        }
    }
}
void segmentation(SDL_Surface *image)
{
    for (int y = 0; y < image->h ; y++) {
        for (int x = 0; x < image->w; x++) {

        }
        
    }
}
