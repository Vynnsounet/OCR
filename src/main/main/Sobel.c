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
            int x = i + col;
            int y = j + row;
            if (x >= 0 && y >= 0 && x < (int)image->w
                && y < (int)image->h)
            {
                Uint8 *pixel = (Uint8 *)(image->pixels) + y * image->pitch + x * image->format->BytesPerPixel;
                sum += pixel[0] * kernel[i][j];
            }
        }
    }

    return sum;
}
void sobel(SDL_Surface* image)
{

    double gx, gy;
    double g;
    double kx[3][3] = { { -1.0, 0.0, 1.0 },
                              { -2.0, 0.0, 2.0 },
                              { -1.0, 0.0, 1.0 } };

    double ky[3][3] = { { -1.0, -2.0, -1.0 },
                              { 0.0, 0.0, 0.0 },
                              { 1.0, 2.0, 1.0 } };

    for (int i = 0; i < image->h; i++)
    {
        for (int j = 0; j < image->w; j++)
        {
            gx = Convolution(image, kx, i, j);
            gy = Convolution(image, ky, i, j);
            g = sqrt(gx * gx + gy * gy);

            Uint8 *pixel = (Uint8 *)(image->pixels) + i * image->pitch + j * image->format->BytesPerPixel;
            Uint8 new_color = (Uint8)(g > 128 ? 255 : 0);

            pixel[0] = new_color;
            pixel[1] = new_color;
            pixel[2] = new_color;
        }
    }
    
}
