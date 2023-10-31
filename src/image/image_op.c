#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

Uint32 get_pixel(SDL_Surface *image, int y, int x)
{
    return *((Uint32 *)((Uint8 *)image->pixels + y * image->pitch + x * 4));
}

void put_pixel(SDL_Surface *image, int y, int x, Uint32 rgba)
{
    *((Uint32 *)((Uint8 *)image->pixels + y * image->pitch + x * 4)) = rgba;
}


void grayscale(SDL_Surface *image)
{
    for (int y = 0; y < image->h; y++)
    {
        for (int x = 0; x < image->w; x++)
        {

            Uint8 red, green, blue;
            Uint32 pixel = get_pixel(image, y, x);
            SDL_GetRGB(pixel, image->format, &red, &green, &blue);

            Uint8 gray = (Uint8)(0.3 * red + 0.58 * green + 0.11 * blue);

            put_pixel(image, y, x, SDL_MapRGB(image->format, gray, gray, gray));
        }
    }
}



void binarize(SDL_Surface *image, Uint8 treshold)
{
    for (int y = 0; y < image->h; y++)
    {
        for (int x = 0; x < image->w; x++)
        {

            Uint8 red, green, blue, alpha;
            Uint32 pixel = get_pixel(image, y, x);

            SDL_GetRGBA(pixel, image->format, &red, &green, &blue, &alpha);

            if ((red + blue + green) / 3 >= treshold)
            {
                put_pixel(image, y, x, SDL_MapRGBA(image->format, 255, 255, 255, alpha));
            }
            else
            {
                put_pixel(image, y, x, SDL_MapRGBA(image->format, 0, 0, 0, alpha));
            }
        }
    }
}
void convolution(SDL_Surface *image, size_t k, float matrix[k][k])
{
    for (size_t y = k / 2; y < (image->h) - k / 2; y++)
    {
        for (size_t x = k / 2; x < (image->w) - k / 2; x++)
        {
            Uint8 acc = 0;
            for (size_t yk = 0; yk < k; yk++)
            {
                for (size_t xk = 0; xk < k; xk++)
                {
                    Uint8 red, green, blue, alpha;
                    Uint32 pixel = get_pixel(image, y - 1 + yk, x - 1 + xk);
                    SDL_GetRGBA(pixel, image->format, &red, &green, &blue, &alpha);
                    acc += red * matrix[yk][xk];
                }
            }
            Uint8 red, green, blue;
            Uint32 pixel = get_pixel(image, y, x);
            SDL_GetRGB(pixel, image->format, &red, &green, &blue);
            put_pixel(image, y, x, SDL_MapRGB(image->format, (Uint8)acc, (Uint8)acc, (Uint8)acc));
        }
    }
}



