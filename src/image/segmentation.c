#include <SDL2/SDL.h>
#include "includes/image_op.h"
#include "includes/hough.h"
#include <math.h>
#include <stdio.h>
#include "includes/image.h"
#include "includes/segmentation.h"



int *positions(int *array, int *length, double average)
{
    int *pos = malloc(10*sizeof(int));
    pos[0] = array[0];
    int count = 1;
    for (int i = 1; i < 10; i++) {
        int prevpos = pos[i - 1];

        int newpos = prevpos + average;
        if(abs(newpos - array[count]) < 20)
        {
            pos[i] = array[count++];
        }
        else
        {
            pos[i] = newpos;
        }
    }
    *length = 10;
    free(array);
    return pos;
}
int* filter(int xpositions[], int n, double uncertainty, double threshold, int *filteredCount,double *weightedAverageDistance) {
    double *distances = malloc((n - 1) * sizeof(double));
    for (int i = 0; i < n - 1; ++i) {
        distances[i] = abs(xpositions[i + 1] - xpositions[i]);
    }

    double weightedSum = 0.0;
    double weightSum = 0.0;

    for (int i = 0; i < n - 1; ++i) {
        double weight = 1.0;

        if (i > 0 && fabs( distances[i] - distances[i - 1]) < uncertainty) {
            weight *= 40;
        }
        if (distances[i]>threshold) {
            weightedSum += weight * distances[i];
            weightSum += weight;

        }
    }

    if (weightSum == 0.0) {
        printf("No valid weights for calculation.\n");
        free(distances);
        *filteredCount = 0;
        return NULL;
    }

    *weightedAverageDistance = weightedSum / weightSum;
    int *filteredPositions = malloc((n - 1) * sizeof(int));
    *filteredCount = 0;


    for (int i = 0; i < n - 1; ++i) {
        if ((distances[i] >= threshold && fabs(distances[i] - *weightedAverageDistance) < uncertainty)) {
            filteredPositions[(*filteredCount)++] = xpositions[i];

        }
    }

    free(distances);
    int *pos = positions(filteredPositions, filteredCount, *weightedAverageDistance);
    return pos;
}
double calculateDistance(double x1, double x2) {
    return fabs(x2 - x1);
}



void detection(SDL_Surface *image, SDL_Surface *seg)
{
    int count = 0;
    int coordx[64];
    int coordy[64];
    for (int x = 1; x < image->w; x++) {
        Uint8 red, green, blue, alpha;
        Uint32 pixel = get_pixel(image, 2, x);
        SDL_GetRGBA(pixel, image->format, &red, &green, &blue, &alpha);
        if (red == 255) {

            coordx[count++] = x;
            if (count==64) {
                break;
            }
            while (red == 255 && x<image->w)
            {
                pixel = get_pixel(image, 2, x);
                SDL_GetRGBA(pixel, image->format, &red, &green, &blue, &alpha);
                x++;
            }
        }




    }
    
    int countx = count;
    count = 0;
    for (int y = 1; y < image->h; y++) {
           Uint8 red, green, blue, alpha;
           Uint32 pixel = get_pixel(image, y, 2);
           SDL_GetRGBA(pixel, image->format, &red, &green, &blue, &alpha);
           if (red == 255) {

               coordy[count++] = y;
               if (count==64) {
                      break;
                  }

               while (red == 255 && y<image->h)
               {
                   pixel = get_pixel(image, y, 2);
                   SDL_GetRGBA(pixel, image->format, &red, &green, &blue, &alpha);
                   y++;

               }
           }



       }
    
    int counterx = 0;
    int countery= 0;
    double averagex = 0;
    double averagey = 0;

    int* cx = filter( coordx, countx, 7, 20,&counterx, &averagex);
    int* cy = filter(coordy, count, 7, 20, &countery, &averagey);
    if(!cx || !cy)
        return;
    for (int y = 0; y < image->h; y++) {
        for (int x = 0; x < image->w; x++) {
            if (x<cx[0]||y<cy[0]||x>cx[9]||y>cy[9]) {
                Uint8 red, green, blue, alpha;
                Uint32 pixel = get_pixel(image, y, x);
                SDL_GetRGBA(pixel, image->format, &red, &green , &blue, &alpha);
                put_pixel(image , y , x, SDL_MapRGBA(image->format, 0, 0, 0, alpha));
                
            }
        }
    }
    segmentation(seg, cx, cy);
    free(cx);
    free(cy);
}

void segmentation(SDL_Surface *image, int* xpos, int* ypos){
    char* str = malloc(11*sizeof(char));
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            int x1 = xpos[x];
            int y1 = ypos[y];
            int x2 = xpos[x+1];
            int y2 = ypos[y+1];
            int width = x2 - x1;
            int height = y2 - y1;
            SDL_Surface* cropped = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);

            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    Uint32 pixel = get_pixel(image, i+y1 , x1+j);
                    put_pixel(cropped, i , j, pixel);
                }
            }
            sprintf(str, "cropped/%d-%d", y+1 , x+1);
            SDL_Surface* resize = SDL_CreateRGBSurface(0, 48, 48, 32, 0, 0, 0, 0);
            SDL_BlitScaled(cropped, NULL, resize, NULL);
            cropped = SDL_CreateRGBSurface(0, 28, 28, 32, 0, 0, 0, 0);
            for (int i = 0; i < 28; i++) {
                for (int j = 0; j < 28; j++) {
                    Uint32 pixel = get_pixel(resize, i+10 , 10+j);
                       put_pixel(cropped, i , j, pixel);
                }
            }

            save_image(cropped, str);

            SDL_FreeSurface(cropped);
            SDL_FreeSurface(resize);
        }

    }
    free(str);
}


