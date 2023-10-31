#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>





double calculateContrastFactor(SDL_Surface *surface, double targetContrast) {
    int width = surface->w;
    int height = surface->h;
    int numPixels = width * height;
    double originalContrast = 0.0;
    double sum = 0.0;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Uint8 *pixel = (Uint8 *)(surface->pixels) + y * surface->pitch + x * surface->format->BytesPerPixel;
            sum += (pixel[0] + pixel[1] + pixel[2]) / 3.0;
        }
    }
    double average = sum / numPixels;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Uint8 *pixel = (Uint8 *)(surface->pixels) + y * surface->pitch + x * surface->format->BytesPerPixel;
            double pixelValue = (pixel[0] + pixel[1] + pixel[2]) / 3.0;
            originalContrast += pow(pixelValue - average, 2);
        }
    }
    originalContrast = sqrt(originalContrast / numPixels);

    
    double factor = targetContrast/originalContrast;

    if (factor < 0.1) {
        factor = 0.1;
    } else if (factor > 2.0) {
        factor = 2.0;
    }

    return factor;
}

void reduceContrast(SDL_Surface *surface, double factor) {

    factor = calculateContrastFactor(surface, factor);


    int width = surface->w;
    int height = surface->h;

    SDL_LockSurface(surface);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Uint8 *pixel = (Uint8 *)(surface->pixels) + y * surface->pitch + x * surface->format->BytesPerPixel;
            
            pixel[0] = (((pixel[0] * factor))>255) ? 255 : (pixel[0] * factor);
            pixel[1] = (((pixel[1] * factor))>255) ? 255 : (pixel[1] * factor);
            pixel[2] = (((pixel[2] * factor))>255) ? 255 : (pixel[2] * factor);
        }
    }

    SDL_UnlockSurface(surface);
}
