#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>
#include "includes/image.h"

Uint32 get_pixel(SDL_Surface *image, int y, int x)
{
    return *((Uint32 *)((Uint8 *)image->pixels + y * image->pitch + x * image->format->BytesPerPixel));
}

void put_pixel(SDL_Surface *image, int y, int x, Uint32 rgba)
{
    *((Uint32 *)((Uint8 *)image->pixels + y * image->pitch + x * image->format->BytesPerPixel)) = rgba;
}


void grayscale(SDL_Surface *image)
{
    for (int y = 0; y < image->h; y++)
    {
        for (int x = 0; x < image->w; x++)
        {

            Uint8 red, green, blue, alpha;
            Uint32 pixel = get_pixel(image, y, x);
            SDL_GetRGBA(pixel, image->format, &red, &green, &blue, &alpha);

            Uint8 gray = (Uint8)(0.3 * red + 0.58 * green + 0.11 * blue);

            put_pixel(image, y, x, SDL_MapRGBA(image->format, gray, gray, gray, alpha));
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

void convolution(SDL_Surface *image, int k, double matrix[k][k])
{
    for (int y = 0; y < (image->h); y++)
    {
        for (int x = 0; x < (image->w); x++)
        {
            Uint8 acc = 0;
            for (int yk = 0; yk < k; yk++)
            {
		if (y - k/2 + yk<=0 || y - k/2 +yk>=image->h)
			continue;
                for (int xk = 0; xk < k; xk++)
                {
			if (x - k/2 + xk<=0 ||( x - k/2 +xk)>=image->w)
				continue;
                    Uint8 red, green, blue, alpha;
                    Uint32 pixel = get_pixel(image, y - k/2 + yk, x - k/2 + xk);
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

double validcell(SDL_Surface * image)
{
  int nb = 0;
  for (int y = 0; y < image->h; y++)
    {
        for (int x = 0; x < image->w; x++)
        {

            Uint8 red, green, blue, alpha;
            Uint32 pixel = get_pixel(image, y, x);
            SDL_GetRGBA(pixel, image->format, &red, &green, &blue, &alpha);
	    if (red <=5 && green <=5)
	      {
		nb+=1;
	      }
	}
    }
  double res = ((double)nb/(image->h*image->w))*100.0;
 
  printf("%f\n",res);
  return res;

}



void createfilesolver(char *directoryPath, char* grid) {

   FILE *gridd = fopen(grid , "w");
   int i = 1;
   int a = 1;
   int b = 1;
   while (a<=9 && b<=9)
     {
       char c[10];
       snprintf(c, sizeof(c), "%i-%i", a, b);

            char filePath[1024];
            snprintf(filePath, sizeof(filePath), "%s/%s", directoryPath, c);
		SDL_Surface *cell = load_image(filePath);
                if (validcell(cell)>5)
		  {
		    fprintf(gridd,"C");
		  }
		else
		  {
		    fprintf(gridd,".");
		  }

		if (i%3==0 && i%9!=0) //&& i!= 30 && i!=33)|| i==31 || i==35)
		  {

		    fprintf(gridd, " ");

		  }
		if (i%9==0)
		  {
		    fprintf(gridd, "\n");
		    
		  }
		i+=1;
       b+=1;
       if (b>9)
	 {
	   a+=1;
	   b=1;
	 }
       
    }
    
    fclose(gridd);
     
}

    
