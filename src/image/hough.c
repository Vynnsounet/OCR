#include <SDL2/SDL.h>


void HoughTransform(SDL_Surface *image, SDL_Renderer* renderer)
{
  int len = ((int)sqrt(image->w*image->w+image->h*image->h));
  
    int accumulator[len][180];
    for (int rho = 0; rho < len; rho++) {
        for (int theta = 0; theta < 180; theta++) {
            accumulator[rho][theta] = 0;
        }
    }

    for (int x = 0; x < image->h; x++) {
        for (int y = 0; y < image->w; y++) {
	   Uint8 red, green, blue, alpha;

	  Uint32 pixel = get_pixel(image, y, x);

	  SDL_GetRGBA(pixel, image->format, &red, &green, &blue, &alpha);
            if (green == 255) {
                for (int theta = 0; theta < 180; theta++) {
                    double thetaRad = theta * M_PI / 180.0;
                    int rho = x * cos(thetaRad) + y * sin(thetaRad);
                    accumulator[rho][theta]++;
                }
            }
        }
    }


    for (int rho = 0; rho < len; rho++) {
        for (int theta = 0; theta < 180; theta++) {
            if (accumulator[rho][theta] >= SEUIL) {

                double thetaRad = theta * M_PI / 180.0;
                int x1 = (int)(rho * cos(thetaRad));
                int y1 = (int)(rho * sin(thetaRad));
                int x2 = x1 + 1000 * (-sin(thetaRad)); 
                int y2 = y1 + 1000 * (cos(thetaRad));  


                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
            }
        }
    }

    SDL_RenderPresent(renderer);


    int quit = 0;
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }
    }
}
