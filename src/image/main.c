#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "includes/image_op.h"
#include "includes/image.h"
#include "includes/threshold.h"
#include "includes/contrast.h"
#include "includes/Sobel.h"
void xmul(size_t k, float array[k][k], float x)
{
    for (size_t i = 0; i < k; ++i)
    {
        for (size_t j = 0; j < k; ++j)
        {
            array[i][j] *= x;
        }
    }
}

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        printf("IMG_Init failed: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Surface *original_image = load_image("pictures/image02");

    if (original_image == NULL)
    {
        SDL_Quit();
        return 1;
    }
    float gauss[5][5] = {{2, 4, 5, 4, 2}, {4, 9, 12, 9, 4}, {5, 12, 15, 12, 5}, {4, 9, 12, 9, 4}, {2, 4, 5, 4, 2}};
    xmul(5, gauss, (float)(1.0 / 159.0));
    grayscale(original_image);
    save_image(original_image, "grayscale.bmp");

    convolution(original_image, 5, gauss);

    save_image(original_image, "gauss.bmp");
    reduceContrast(original_image, 50);
    sobel(original_image);
    save_image(original_image, "sobel.bmp");

    //reduceContrast(original_image, 1.8);
    //save_image(original_image, "contrast.bmp");

    //Uint8 otsuThreshold = calculateOtsuThreshold(original_image);
    binarize(original_image, 200);

    save_image(original_image, "binarized_image.bmp");

    //keepLargestComponent(original_image);
    //save_image(original_image, "crop.bmp");

    SDL_FreeSurface(original_image);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
