#include "includes/image.h"


SDL_Surface *load_image(char *filename)
{
    SDL_Surface *image = IMG_Load(filename);
    if (image == NULL)
    {
        printf("The file does not exist\n");
    }
    return image;
}



void save_image(SDL_Surface *image, const char *filename)
{
    SDL_SaveBMP(image, filename);
}

void resize_image_GTK(SDL_Surface* image)
{
    SDL_Surface* resize = SDL_CreateRGBSurface(0, 550, 550, 32, 0, 0, 0, 0);
    SDL_BlitScaled(image, NULL, resize, NULL);
    save_image(resize, "processedresized.bmp");
    SDL_FreeSurface(resize);
    SDL_FreeSurface(image);
}
