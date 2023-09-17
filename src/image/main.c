#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Function to load an image and return a SDL_Surface*
SDL_Surface* load_image(char* filename) {
    SDL_Surface* image = IMG_Load(filename);
    if (image == NULL) {
        printf("Nope");
    }
    return image;
}




void convert_to_grayscale(SDL_Surface* image) {
    for (int y = 0; y < image->h; y++) {
        for (int x = 0; x < image->w; x++) {

            Uint8 red, green, blue, alpha;
            SDL_GetRGBA(*((Uint32*)((Uint8*)image->pixels + y * image->pitch + x * 4)), image->format, &red, &green, &blue, &alpha);

            Uint8 gray = (Uint8)(0.3 * red + 0.58 * green + 0.11 * blue);

            *((Uint32*)((Uint8*)image->pixels + y * image->pitch + x * 4)) = SDL_MapRGBA(image->format, gray, gray, gray, alpha);
        }
    }
}



void binarize(SDL_Surface* image){
	for (int y = 0; y < image->h; y++) {
        	for (int x = 0; x < image->w; x++) {

            Uint8 red, green, blue, alpha;
            SDL_GetRGBA(*((Uint32*)((Uint8*)image->pixels + y * image->pitch + x * 4)), image->format, &red, &green, &blue, &alpha);

	    if ((red +blue+green)/3 >= 127)
	    {
		*((Uint32*)((Uint8*)image->pixels + y * image->pitch + x * 4)) = SDL_MapRGBA(image->format, 255, 255, 255, alpha);
	    }
	    else
	    {
		    *((Uint32*)((Uint8*)image->pixels + y * image->pitch + x * 4)) = SDL_MapRGBA(image->format, 0, 0, 0, alpha);
	    }
        }
    }
}

void save_image(SDL_Surface* image, const char* filename) {
    SDL_SaveBMP(image, filename);
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("IMG_Init failed: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Surface* original_image = load_image("cat_PNG50479.png");
    if (original_image == NULL) {
        SDL_Quit();
        return 1;
    }

    convert_to_grayscale(original_image);
    save_image(original_image, "grayscale_image.bmp");
    binarize(original_image);
    save_image(original_image, "binarized_image.bmp");

    SDL_FreeSurface(original_image);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
