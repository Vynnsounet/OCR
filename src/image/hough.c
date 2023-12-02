#include <SDL2/SDL.h>
#include "includes/image_op.h"

#define SEUIL 400



void drawLine(int x0, int y0, int x1, int y1, SDL_Surface * image) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int steps =0;
    if (abs(dx)>abs(dy))
      {
	steps = abs(dx);
      }
    else
      {steps = abs(dy);}

    float xI = (float)dx / (float)steps;
    float yI = (float)dy / (float)steps;
    

    /*
    while (x0<1)
      {
	x0+=abs((int)xI);
      }
    while (y0<1)
      {
	y0+=abs((int)yI);
      }
    */
		  
 

    int x = x0;
    int y = y0;

    while (x<image->w  && y<image->h && x>0  && y>0) {
          Uint8 red, green, blue, alpha;

	  Uint32 pixel = get_pixel(image, y, x);

	  SDL_GetRGBA(pixel, image->format, &red, &green, &blue, &alpha);
	  put_pixel(image, y, x, SDL_MapRGBA(image->format, 255, 0, 0, alpha));
	  
          // Assuming 255 represents a white pixel
	  x +=xI;
	  y +=yI;
    }
    x=x0;
    y=y0;

    while (x>0  && y>0 && x<image->w  && y<image->h) {
          Uint8 red, green, blue, alpha;

	  Uint32 pixel = get_pixel(image, y, x);

	  SDL_GetRGBA(pixel, image->format, &red, &green, &blue, &alpha);
	  put_pixel(image, y, x, SDL_MapRGBA(image->format, 255, 0, 0, alpha));
	  
          // Assuming 255 represents a white pixel
	  x -=xI;
	  y -=yI;
    }
    
}

void HoughTransform(SDL_Surface *image)
{
  int len = ((int)sqrt(image->w*image->w+image->h*image->h));
  int seuil = 0;
  
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
		    if (accumulator[rho][theta]>seuil)
		      {seuil = accumulator[rho][theta];}
                }
            }
        }
    }


    for (int rho = 0; rho < len; rho++) {
        for (int theta = 0; theta < 180; theta++)
	  {
            if (accumulator[rho][theta] >= SEUIL)
	      {

                double thetaRad = theta * M_PI / 180.0;
                int x1 = (int)(rho * cos(thetaRad));
                int y1 = (int)(rho * sin(thetaRad));
                int x2 = x1 + 1000 * (-sin(thetaRad)); 
                int y2 = y1 + 1000 * (cos(thetaRad));		
		int cor = 0;
		 while (x1<1)
		  {
		    x1+=1;
		    x2+=1;
		    cor=1;
		  }
		  while (y1<1)
		  {
		    y1+=1;
		    y2+=1;
		    cor=1;
		  }
		  if (cor==1)
		    {

		  drawLine(x1, y1, x2, y2, image);
		    }
		
            }
        }
    }
}



