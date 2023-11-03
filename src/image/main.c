#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "includes/image_op.h"
#include "includes/image.h"
#include "includes/threshold.h"
#include "includes/contrast.h"
#include "includes/Sobel.h"
void xmul(size_t k, double array[k][k], float x)
{
    for (size_t i = 0; i < k; ++i)
    {
        for (size_t j = 0; j < k; ++j)
        {
            array[i][j] *= x;
        }
    }
}

int main(int argc, char** argv)
{
	if (argc !=2)
	{
		printf("error: wrong number of arguments\n");
		return 1;
	}
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

	SDL_Surface *original_image = load_image(argv[1]);

	if (original_image == NULL)
	{
		SDL_Quit();
		return 1;
	}
	double gauss[5][5] = {{2, 4, 5, 4, 2}, {4, 9, 12, 9, 4}, {5, 12, 15, 12, 5}, {4, 9, 12, 9, 4}, {2, 4, 5, 4, 2}};
	xmul(5, gauss, (double)(1.0 / 159.0));
	grayscale(original_image);
	save_image(original_image, "grayscale.bmp");

	convolution(original_image, 5, gauss);

	save_image(original_image, "gauss.bmp");

	reduceContrast(original_image, 40);

	//reduceContrast(original_image, 50);
	//save_image(original_image, "contrast.bmp");
	sobel(original_image);
	save_image(original_image, "sobel.bmp");
	//Uint8 otsuThreshold = calculateOtsuThreshold(original_image);
	

	SDL_FreeSurface(original_image);
	IMG_Quit();
	SDL_Quit();

	return 0;
}
