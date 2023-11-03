#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "includes/image_op.h"




Uint8 calculateOtsuThreshold(SDL_Surface *image)
{
    int histogram[255] = {0};
    for (int y = 0; y < image->h; y++)
    {
        for (int x = 0; x < image->w; x++)
        {
            Uint8 red, green, blue, alpha;
            Uint32 pixel = get_pixel(image, y, x);
            SDL_GetRGBA(pixel, image->format, &red, &green, &blue, &alpha);

            Uint8 gray = (Uint8)(0.3 * red + 0.58 * green + 0.11 * blue);

            histogram[gray]++;
        }
    }

    int totalPixels = image->w * image->h;

    double sum = 0.0;
    for (int i = 0; i < 255; i++)
    {
        sum += i * histogram[i];
    }

    double sumB = 0.0;
    double wB = 0.0;
    double wF = 0.0;
    double maxVariance = 0.0;
    Uint8 threshold = 0;

    for (int i = 0; i < 255; i++)
    {
        wB += histogram[i];
        if (wB == 0)
            continue;

        wF = totalPixels - wB;
        if (wF == 0)
            break;

        sumB += i * histogram[i];
        double mB = sumB / wB;
        double mF = (sum - sumB) / wF;
        double varianceBetween = wB * wF * (mB - mF) * (mB - mF);

        if (varianceBetween > maxVariance)
        {
            maxVariance = varianceBetween;
            threshold = i;
        }
    }

    return threshold-30;
}
