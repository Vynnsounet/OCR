#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

SDL_Surface* load_image(char* filename) {
    SDL_Surface* image = IMG_Load(filename);
    if (image == NULL) {
        printf("Nope");
    }
    return image;
}


void xmul(size_t k ,float array[k][k], float x)
{
    for (size_t i = 0; i < k; ++i)
    {
        for (size_t j = 0; j <k; ++j)
        {
            array[i][j] *= x;
        }
 }
}


Uint32 get_pixel(SDL_Surface* image, int y, int x)
{
        return *((Uint32*)((Uint8*)image->pixels + y * image->pitch + x * 4));
}


void put_pixel(SDL_Surface* image, int y, int x, Uint32 rgba)
{
        *((Uint32*)((Uint8*)image->pixels + y * image->pitch + x * 4)) = rgba;
}



void convert_to_grayscale(SDL_Surface* image) {
    for (int y = 0; y < image->h; y++) {
        for (int x = 0; x < image->w; x++) {

            Uint8 red, green, blue, alpha;
	    Uint32 pixel = get_pixel(image, y , x);
            SDL_GetRGBA(pixel , image->format, &red, &green, &blue, &alpha);

            Uint8 gray = (Uint8)(0.3 * red + 0.58 * green + 0.11 * blue);

            put_pixel(image, y, x, SDL_MapRGBA(image->format, gray, gray, gray, alpha));
        }
    }
}



void binarize(SDL_Surface* image, Uint8 treshold){
	for (int y = 0; y < image->h; y++) {
        	for (int x = 0; x < image->w; x++) {

            Uint8 red, green, blue, alpha;
	    Uint32 pixel = get_pixel(image, y , x);

            SDL_GetRGBA(pixel , image->format, &red, &green, &blue, &alpha);

	    if ((red +blue+green)/3 >= treshold)
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
void convolution(SDL_Surface* image, size_t k,float matrix[k][k]){
	for (size_t y = k/2;y< (image->h)-k/2;y++){
		for (size_t x = k/2;x< (image->w)-k/2;x++){
			Uint8 acc= 0;
			for (size_t yk = 0; yk < k; yk++) {
                		for (size_t xk = 0; xk < k; xk++) {
					Uint8 red, green, blue, alpha;
					Uint32 pixel = get_pixel(image, y-1+yk , x-1+xk);
					SDL_GetRGBA(pixel , image->format, &red, &green, &blue, &alpha);
					acc+= red*matrix[yk][xk];

				}
			}
			Uint8 red, green, blue, alpha;
			Uint32 pixel = get_pixel(image, y, x);
			SDL_GetRGBA(pixel , image->format, &red, &green, &blue, &alpha);
			put_pixel(image, y, x, SDL_MapRGBA(image->format, (Uint8)acc, (Uint8)acc, (Uint8)acc, alpha));
		}
	}

}
Uint8 calculateOtsuThreshold(SDL_Surface* image) {
    int histogram[255] = {0};
    for (int y = 0; y < image->h; y++) {
        for (int x = 0; x < image->w; x++) {
            Uint8 red, green, blue, alpha;
            Uint32 pixel = get_pixel(image, y , x);
            SDL_GetRGBA(pixel , image->format, &red, &green, &blue, &alpha);

            Uint8 gray = (Uint8)(0.3 * red + 0.58 * green + 0.11 * blue);

            histogram[gray]++;
        }
    }

    int totalPixels = image->w * image->h;

    double sum = 0.0;
    for (int i = 0; i < 255; i++) {
        sum += i * histogram[i];
    }

    double sumB = 0.0;
    double wB = 0.0;
    double wF = 0.0;
    double maxVariance = 0.0;
    Uint8 threshold = 0;

    for (int i = 0; i < 255; i++) {
        wB += histogram[i];
        if (wB == 0) continue;

        wF = totalPixels - wB;
        if (wF == 0) break;

        sumB += i * histogram[i];
        double mB = sumB / wB;
        double mF = (sum - sumB) / wF;
        double varianceBetween = wB * wF * (mB - mF) * (mB - mF);

        if (varianceBetween > maxVariance) {
            maxVariance = varianceBetween;
            threshold = i;
        }
    }

    return threshold;
}
void save_image(SDL_Surface* image, const char* filename) {
    SDL_SaveBMP(image, filename);
}

SDL_Surface* rotate(SDL_Surface* image, double angle)
{
        SDL_Surface* new_image = image;
        for (int y = 0; y < image->h; y++) {
        for (int x = 0; x < image->w; x++) {

            Uint8 red, green, blue, alpha;
            Uint32 pixel = get_pixel(image, y , x);
            int y1 = x*(int)sin(angle)+y*(int)cos(angle);
            int x1 = x*(int)cos(angle) - y*(int)sin(angle);

            SDL_GetRGBA(pixel , image->format, &red, &green, &blue, &alpha);
            if(y1 <0 || y1>image->h||x1<0 ||x1>image->w)
            {
                    continue;
            }
            put_pixel(new_image, y1, x1, SDL_MapRGBA(new_image->format, red, green, blue, alpha));
        }
    }
        return new_image;
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

    SDL_Surface* original_image = load_image("image_06.png");
    if (original_image == NULL) {
        SDL_Quit();
        return 1;
    }
    float mat[5][5] = {
        {2,4,5,4,2},
        {4, 9, 12,9,4},
        {5, 12, 15, 12,5},
	{4, 9, 12,9,4},
	{2,4,5,4,2}
    };
    xmul(5, mat, (float)(1.0/159.0));
    convert_to_grayscale(original_image);
    save_image(original_image, "grayscale_image.bmp");
    convolution(original_image, 5, mat);
    save_image(original_image, "convoluted_image.bmp");
    Uint8 otsuThreshold = calculateOtsuThreshold(original_image);
    binarize(original_image, otsuThreshold);
    SDL_Surface* image = rotate(original_image, 90.0);
    save_image(image,"rotated_image.bmp");

    save_image(original_image, "binarized_image.bmp");

    SDL_FreeSurface(original_image);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
