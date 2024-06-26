#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "includes/image_op.h"
#include "includes/image.h"
#include "includes/threshold.h"
#include "includes/contrast.h"
#include "includes/Sobel.h"
#include "includes/hough.h"
#include "includes/segmentation.h"
#include "includes/rotation.h"
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

SDL_Surface* convertSurfaceToRGBA8888(SDL_Surface* originalSurface) {
    // Create a pixel format with RGBA8888
    SDL_PixelFormat rgbaFormat = {
        .format = SDL_PIXELFORMAT_RGBA8888,
        .BitsPerPixel = 32,
        .BytesPerPixel = 4,
        .Rmask = 0x000000FF,
        .Gmask = 0x0000FF00,
        .Bmask = 0x00FF0000,
        .Amask = 0xFF000000
    };

    // Create a new surface with RGBA8888 format
    SDL_Surface* convertedSurface = SDL_ConvertSurface(originalSurface, &rgbaFormat, 0);

    if (!convertedSurface) {
        // Handle error, e.g., print an error message or return NULL
        SDL_Log("Failed to convert surface: %s", SDL_GetError());
        return NULL;
    }

    return convertedSurface;
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

	SDL_Surface *hough = load_image(argv[1]);
	hough = convertSurfaceToRGBA8888(hough);

	SDL_Surface *seg = load_image(argv[1]);
	seg = convertSurfaceToRGBA8888(seg);

	if (!hough || !seg)
	{
		SDL_Quit();
		return 1;
	}
	double gauss[5][5] = {{2, 4, 5, 4, 2}, {4, 9, 12, 9, 4}, {5, 12, 15, 12, 5}, {4, 9, 12, 9, 4}, {2, 4, 5, 4, 2}};
	xmul(5, gauss, (double)(1.0 / 159.0));
	grayscale(hough);
	save_image(hough, "grayscale.bmp");

	convolution(seg, 5, gauss);

	save_image(seg, "gauss.bmp");
    grayscale(seg);
	reduceContrast(hough, 40);

	reduceContrast(seg, 40);
	save_image(seg, "contrast.bmp");
	sobel(hough);
	save_image(hough, "sobel.bmp");
	Uint8 otsuThreshold = calculateOtsuThreshold(seg);
    binarize(seg, otsuThreshold);
    save_image(seg, "bin.bmp");

    int z = automaticrotation(hough);
    if (z!=0)
      {
	hough = rotation(z,hough);
	seg = rotation(z,hough);
      }
    
    HoughTransform(hough);
    save_image(hough,"hough.bmp");
    detection(hough, seg);
    save_image(hough, "detection.bmp");

    createfilesolver("cropped/","grid");

	SDL_FreeSurface(hough);
	SDL_FreeSurface(seg);
	IMG_Quit();
	SDL_Quit();

	return 0;
}
