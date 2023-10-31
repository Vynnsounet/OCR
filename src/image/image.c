#include "includes/image.h"



SDL_Surface *load_image(char *filename)
{
    SDL_Surface *image = IMG_Load(filename);
    if (image == NULL)
    {
        printf("Nope");
    }
    return image;
}



void save_image(SDL_Surface *image, const char *filename)
{
    SDL_SaveBMP(image, filename);
}
