#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>


double Convolution(SDL_Surface* image, double kernel[3][3], int row, int col)
{
    double sum = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            int x = i + row;
            int y = j + col;
            if (x >= 0 && y >= 0 && x < (int)image->h
                && y < (int)image->w)
            {
                Uint8 *pixel = (Uint8 *)(image->pixels) + x * image->pitch + y * image->format->BytesPerPixel;
                sum += pixel[0] * kernel[i][j];
            }
        }
    }

    return sum;
}
void Sobel(SDL_Surface* image)
{

    double gx, gy;
    double g;
    double kx[3][3] = { { -1.0, 0.0, 1.0 },
                              { -2.0, 0.0, 2.0 },
                              { -1.0, 0.0, 1.0 } };

    double ky[3][3] = { { -1.0, -2.0, -1.0 },
                              { 0.0, 0.0, 0.0 },
                              { 1.0, 2.0, 1.0 } };

    for (int y = 0; y < image->h; y++)
    {
        for (int x = 0; x < image->w; x++)
        {
            gx = Convolution(image, kx, x, y);
            gy = Convolution(image, ky, x, y);
            g = sqrt(gx * gx + gy * gy);
            Uint8 *pixel = (Uint8 *)(image->pixels) + y * image->pitch + x * image->format->BytesPerPixel;
            pixel[0] = (Uint8)(g>128 ? 255: 0);
            pixel[1] = (Uint8)(g>128 ? 255: 0);
            pixel[2] = (Uint8)(g>128 ? 255: 0);

        }
    }
    
}
