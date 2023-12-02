#include <SDL2/SDL.h>
#include "includes/image_op.h"
#include "includes/hough.h"
#include <math.h>
#include <stdio.h>
#include "includes/image.h"
#include "includes/segmentation.h"
void printArray(int* array, int length) {
    if (array == NULL || length <= 0) {
        printf("Invalid array\n");
        return;
    }

    printf("Array: ");
    for (int i = 0; i < length; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}
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
            weight *= 40; // You can adjust the weight factor as needed
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
    printf("average: %2f\n", *weightedAverageDistance);
    int *filteredPositions = malloc((n - 1) * sizeof(int));
    *filteredCount = 0;


    for (int i = 0; i < n - 1; ++i) {
        if ((distances[i] >= threshold && fabs(distances[i] - *weightedAverageDistance) < uncertainty)) {
            filteredPositions[(*filteredCount)++] = xpositions[i];
            printf("%d ", xpositions[i]);
            printf("%d\n", xpositions[i + 1]);

        }
    }

    free(distances);
    int *pos = positions(filteredPositions, filteredCount, *weightedAverageDistance);
    return pos;
}
int* filterByStandardDeviation(int xpositions[], int n, double uncertainty, double threshold, int *filteredCount) {
    double *distances = malloc((n - 1) * sizeof(double));
    for (int i = 0; i < n - 1; ++i) {
        distances[i] = abs(xpositions[i + 1] - xpositions[i]);
    }

    double mean = 0.0;
    for (int i = 0; i < n - 1; ++i) {
        mean += distances[i];
    }
    mean /= (n - 1);

    double stdDeviation = 0.0;
    for (int i = 0; i < n - 1; ++i) {
        stdDeviation += pow(distances[i] - mean, 2);
    }
    stdDeviation = sqrt(stdDeviation / (n - 1));

    int *filteredPositions = malloc((n - 1) * sizeof(int));
    *filteredCount = 0;

    for (int i = 0; i < n - 1; ++i) {
        if (distances[i] >= threshold && distances[i] - mean < uncertainty * stdDeviation) {
            filteredPositions[(*filteredCount)++] = xpositions[i];
            printf("%d ", xpositions[i]);
             printf("%d\n", xpositions[i + 1]);
        }
    }

    free(distances);
    return filteredPositions;
}
double calculateDistance(double x1, double x2) {
    return fabs(x2 - x1);
}

void filterByMostCommonDistance(double xpositions[], int n, double uncertainty) {
    double distances[n][n];
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            distances[i][j] = fabs(xpositions[i] - xpositions[j]);
        }
    }

    int count[n * n];
    for (int i = 0; i < n * n; ++i) {
        count[i] = 0;
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            ++count[(int)(distances[i][j] / uncertainty)];
        }
    }

    int mostCommonDistance = 0;
    for (int i = 1; i < n * n; ++i) {
        if (count[i] > count[mostCommonDistance]) {
            mostCommonDistance = i;
        }
    }

    printf("Most common distance: %d * uncertainty\n", mostCommonDistance);
    printf("Filtered x positions:\n");

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if ((int)(distances[i][j] / uncertainty) == mostCommonDistance) {
                printf("%.2f ", xpositions[i]);
                printf("%.2f\n", xpositions[j]);
            }
        }
    }
}
int* filterByAverageDistance(int xpositions[], int n, double uncertainty, double threshold, int *filteredCount) {
    double *distances = malloc((n - 1) * sizeof(double));
    for (int i = 0; i < n - 1; ++i) {
        distances[i] = abs(xpositions[i + 1] - xpositions[i]);
    }

    double totalDistance = 0.0;
    int pairCount = 0;

    for (int i = 0; i < n - 1; ++i) {
        if (distances[i] >= threshold) {
            totalDistance += distances[i];
            pairCount++;
        }
    }

    if (pairCount == 0) {
        printf("No distances above the threshold for calculation.\n");
        free(distances);
        *filteredCount = 0;
        return NULL;
    }

    double averageDistance = totalDistance / pairCount;
    printf("The average distance is: %.2f\n", averageDistance);
    int *filteredPositions = malloc(pairCount * sizeof(int));
    *filteredCount = 0;

    for (int i = 0; i < n - 1; ++i) {
        if (distances[i] >= threshold && fabs(distances[i] - averageDistance) < uncertainty) {
            filteredPositions[(*filteredCount)++] = xpositions[i];
            printf("%d ", xpositions[i]);
            printf("%d\n", xpositions[i + 1]);
        }
    }

    free(distances);
    return filteredPositions;
}
double distance(double positions[],int n, double constant_uncertainty) {
    double weights[n];
    double weighted_mean = 0.0;
    double weighted_distances[n];
    double most_probable_distance = 0.0;

    for (int i = 0; i < n; i++) {
        weights[i] = 1 / pow(constant_uncertainty, 2);
        weighted_mean += weights[i] * positions[i];
    }
    weighted_mean /= (double) n;

    for (int i = 0; i < n; i++) {
        weighted_distances[i] = weights[i] * pow(positions[i] - weighted_mean, 2);
        most_probable_distance += weighted_distances[i];
    }
    most_probable_distance /= (double) n;

    return sqrt(most_probable_distance);
}


void shiftLeft(double arr[], int size) {
    if (size <= 1) {
        return;
    }


    for (int i = 0; i < size - 1; ++i) {
        arr[i] = arr[i + 1];
    }

}
void detection(SDL_Surface *image, SDL_Surface *seg)
{
    int count = 0;
    int coordx[64];
    int coordy[64];
    for (int x = 0; x < image->w; x++) {
        Uint8 red, green, blue, alpha;
        Uint32 pixel = get_pixel(image, 4, x);
        SDL_GetRGBA(pixel, image->format, &red, &green, &blue, &alpha);
        if (red == 255) {

            coordx[count++] = x;
            if (count==64) {
                break;
            }
            while (red == 255 && x<image->w)
            {
                pixel = get_pixel(image, 4, x);
                SDL_GetRGBA(pixel, image->format, &red, &green, &blue, &alpha);
                x++;
            }
        }




    }
    
    printf("jid\n");
    int countx = count;
    count = 0;
    for (int y = 1; y < image->h; y++) {
           Uint8 red, green, blue, alpha;
           Uint32 pixel = get_pixel(image, y, 4);
           SDL_GetRGBA(pixel, image->format, &red, &green, &blue, &alpha);
           if (red == 255) {

               coordy[count++] = y;
               if (count==64) {
                      break;
                  }

               while (red == 255 && y<image->h)
               {
                   pixel = get_pixel(image, y, 4);
                   SDL_GetRGBA(pixel, image->format, &red, &green, &blue, &alpha);
                   y++;

               }
           }



       }
    
    printf("jid\n");
    //double dist = distance(coordx, (countx == 10) ? 10 :countx+1, 30);
    //printf("jiefj");
    //while(fabs(dist - fabs(coordx[1]-coordx[0]))>40)
    //{
      //  shiftLeft(coordx, (countx == 10) ? 10 :countx+1);
    //}
    int counterx = 0;
    int countery= 0;
    double averagex = 0;
    double averagey = 0;
    printf("jid\n");

    int* cx = filter( coordx, countx, 7, 20,&counterx, &averagex);
    printArray(cx, counterx);
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
            save_image(cropped, str);
            SDL_FreeSurface(cropped);
        }

    }
    free(str);
}
