#include <SDL2/SDL.h>
#include "includes/image_op.h"
#include "includes/hough.h"
#include <math.h>
#include <time.h>
#include <stdio.h>



int* filterByWeightedAverageDistance(int xpositions[], int n, double uncertainty, double threshold, int *filteredCount) {
    // Calculate pair-wise distances
    double *distances = malloc((n - 1) * sizeof(double));
    for (int i = 0; i < n - 1; ++i) {
        distances[i] = fabs(xpositions[i + 1] - xpositions[i]);
    }

    // Calculate the weighted sum of distances
    double weightedSum = 0.0;
    double weightSum = 0.0;

    for (int i = 0; i < n - 1; ++i) {
        double weight = 1.0;

        // Adjust weight for consecutive xpositions that are more or less equal
        if (i > 0 && fabs( distances[i] - distances[i - 1]) < uncertainty) {
            weight *= 2.0; // You can adjust the weight factor as needed
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

    double weightedAverageDistance = weightedSum / weightSum;
    printf("average: %2f\n", weightedAverageDistance);
    // Filter positions based on the weighted average distance
    int *filteredPositions = malloc((n - 1) * sizeof(int));
    *filteredCount = 0;


    for (int i = 0; i < n - 1; ++i) {
        if (distances[i] >= threshold && distances[i] - weightedAverageDistance < uncertainty) {
            // Check if the current position is not near the last added position
            filteredPositions[(*filteredCount)++] = xpositions[i];
            printf("%d ", xpositions[i]);
            printf("%d\n", xpositions[i + 1]);

        }
    }


    free(distances);
    return filteredPositions;
}
int* filterByStandardDeviation(int xpositions[], int n, double uncertainty, double threshold, int *filteredCount) {
    // Calculate pair-wise distances
    double *distances = malloc((n - 1) * sizeof(double));
    for (int i = 0; i < n - 1; ++i) {
        distances[i] = fabs(xpositions[i + 1] - xpositions[i]);
    }

    // Calculate the mean of distances
    double mean = 0.0;
    for (int i = 0; i < n - 1; ++i) {
        mean += distances[i];
    }
    mean /= (n - 1);

    // Calculate the standard deviation
    double stdDeviation = 0.0;
    for (int i = 0; i < n - 1; ++i) {
        stdDeviation += pow(distances[i] - mean, 2);
    }
    stdDeviation = sqrt(stdDeviation / (n - 1));

    // Filter positions based on the standard deviation
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
    // Calculate pair-wise distances
    double distances[n][n];
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            distances[i][j] = fabs(xpositions[i] - xpositions[j]);
        }
    }

    // Count occurrences of each distance
    int count[n * n];
    for (int i = 0; i < n * n; ++i) {
        count[i] = 0;
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            ++count[(int)(distances[i][j] / uncertainty)];
        }
    }

    // Find the most common distance
    int mostCommonDistance = 0;
    for (int i = 1; i < n * n; ++i) {
        if (count[i] > count[mostCommonDistance]) {
            mostCommonDistance = i;
        }
    }

    // Filter positions based on the most common distance
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
    // Calculate pair-wise distances
    double *distances = malloc((n - 1) * sizeof(double));
    for (int i = 0; i < n - 1; ++i) {
        distances[i] = fabs(xpositions[i + 1] - xpositions[i]);
    }

    // Calculate the average distance
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
    // Filter positions based on the average distance
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
void detection(SDL_Surface *image)
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
    printf("jid\n");

    int* cx = filterByWeightedAverageDistance( coordx, countx, 50, 30,&counterx);
    int* cy = filterByWeightedAverageDistance(coordy, count, 50, 30, &countery);

    for (int y = 0; y < image->h; y++) {
        for (int x = 0; x < image->w; x++) {
            if (x<cx[0]||y<cy[0]) {
                Uint8 red, green, blue, alpha;
                Uint32 pixel = get_pixel(image, y, x);
                SDL_GetRGBA(pixel, image->format, &red, &green , &blue, &alpha);
                put_pixel(image , y , x, SDL_MapRGBA(image->format, 0, 0, 0, alpha));
                
            }
        }
    }
    free(cx);
    free(cy);
}

void segmentation(SDL_Surface *image)
{
    for (int y = 0; y < image->h ; y++) {
        for (int x = 0; x < image->w; x++) {

        }
        
    }
}
