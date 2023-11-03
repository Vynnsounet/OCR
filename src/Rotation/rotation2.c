#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float conversion( float x)
{
	float z = (int) x % 360;
	float pi = 3.14159265359;
	return (z*pi)/180;
}


int max(int a, int b)
{
    if(a > b)
    {
        return a;
    }
    return b;
}

Uint8 * ref(SDL_Surface *surface, int y, int x)
{
	Uint8 * const target_pixel = ((Uint8 *) surface->pixels
                                             + y * surface->pitch
                                             + x * surface->format->BytesPerPixel);
  	return target_pixel;
}

Uint32 get_pix(SDL_Surface *image, int x, int y)
{
 Uint8 *p = ref(image, x, y);

    switch (image->format->BytesPerPixel)
    {
        case 1:
            return *p;

        case 2:
            return *(Uint16 *)p;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;

        case 4:
            return *(Uint32 *)p;
    }

    return 0;
}

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel)
{
    Uint8 *p = ref(surface, x, y);

    switch(surface->format->BytesPerPixel)
    {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else
            {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}


SDL_Surface* size(float a, int h, int w)
{
    if(a == 90)
    {
        SDL_Surface* new = SDL_CreateRGBSurface(0, w*2 , h*2, 32, 0, 0, 0, 0);
        return new;
    }
    else
    {
    float an = conversion(a);
    int wx = -1 *(cos(an)*(w) + sin(an)*0);
    int wy = -1 *(sin(an)*0 - cos(an)*(w));
    int hx =-1*(cos(an)*(w) + sin(an)*(h));
    int hy = -1 *(sin(an)*0 - cos(an)*(w));
    SDL_Surface* new = SDL_CreateRGBSurface(0, sqrt((wx -wy) *(wx - wy) + (hx-hy) * (hx-hy )) , sqrt((wx -wy) *(wx - wy) + (hx-hy) * (hx-hy )), 32, 0, 0, 0, 0);
    return new;
    }
    
}

SDL_Surface *rotation(float angle, SDL_Surface* image)
{
   int height = image->h;
   int width = image->w;
   float a = conversion(angle);
   int maximum = max(height,width);
   //int x1 = -1 *(cos(a)*(width) + sin(a)*0);
   //int x2 = -1*(cos(a)*(width) + sin(a)*(height));
   //int y1 = -1 *(sin(a)*0 - cos(a)*(width));
   //int y2 = -1 *(sin(a)*height -cos(a)*0);
   //int new = sqrt((x2 - x1)*(x2 - x1) + (y1 - y2)*(y1-y2));
   int middle = maximum /2;
   SDL_Surface* newimage = SDL_CreateRGBSurface(0, maximum*2, maximum*2, 32, 0, 0, 0, 0);
   //int he = newimage->h;
   //int wi = newimage->w;

    for(int i =0; i < maximum*2; i++)
    {
        for(int j=0; j< maximum*2; j++)
        {	
            
            int x = middle + (cos(a)*(i-maximum) - sin(a)*(j-maximum));
            int y = middle + (sin(a)*(i-maximum) + cos(a)*(j-maximum));
            
            if(x < height && y < width && y >= 0 && x >= 0)
            {
                Uint32 pix = get_pix(image, x,y);
                put_pixel(newimage, i, j, pix);

            }
        }
    }
   return newimage;
}

SDL_Surface* load_image(const char* path)
{
    SDL_Surface * tmp =  IMG_Load(path);
    SDL_Surface * res = SDL_ConvertSurfaceFormat(tmp, SDL_PIXELFORMAT_RGB888, 0);
    SDL_FreeSurface(tmp);
    return res;
}

int main(int argc, char** argv)
{
    // Checks the number of arguments.
   if (argc != 3)
   {
        printf("Il faut mettre 3 arguments\n Nom du fichier et Angle de Rotation");
        return 1;
   }


    
        
    SDL_Surface* s =load_image(argv[1]);
    
     
    SDL_Surface* new_image = rotation(atof(argv[2]), s);
     
     
    IMG_SavePNG(new_image, "Test.png");
     
    SDL_FreeSurface(s);
    SDL_FreeSurface(new_image);
     
     

    return EXIT_SUCCESS;
}
