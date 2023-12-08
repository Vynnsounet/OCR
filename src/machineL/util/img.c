#include "img.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCHAR 10000

Uint32 get_pixel(SDL_Surface *image, int y, int x)
{
    return *((Uint32 *)((Uint8 *)image->pixels + y * image->pitch + x * image->format->BytesPerPixel));
}

void img_print(Img *img)
{
    matrix_print(img->img_data);
    printf("Img Label: %d\n", img->label);
}

Img *file_to_img(char *filename)
{
    SDL_Surface *s = SDL_LoadBMP(filename);
    if (s == NULL)
    {
        fprintf(stderr, "couldn't load image");
        exit(1);
    }
    Img *res = calloc(1, sizeof(Img));
    res->img_data = matrix_create(28, 28);
    Uint8 r, g, b;
    for (int i = 0; i < 28; i++)
    {
        for (int j = 0; j < 28; j++)
        {
            Uint32 currpix = get_pixel(s, i, j);
            SDL_GetRGB(currpix, s->format, &r, &g, &b);
            res->img_data->entries[i][j] = r == 255 ? 0.0 : 255.0;
        }
    }
    SDL_FreeSurface(s);
    img_print(res);
    return res;
}

Img **csv_to_imgs(char *file_string, int number_of_imgs)
{
    FILE *fp;
    Img **imgs = malloc(number_of_imgs * sizeof(Img *));
    char row[MAXCHAR];
    fp = fopen(file_string, "r");

    // Read the first line
    fgets(row, MAXCHAR, fp);
    int i = 0;
    while (feof(fp) != 1 && i < number_of_imgs)
    {
        imgs[i] = malloc(sizeof(Img));

        int j = 0;
        fgets(row, MAXCHAR, fp);
        char *token = strtok(row, ",");
        imgs[i]->img_data = matrix_create(28, 28);
        while (token != NULL)
        {
            if (j == 0)
            {
                imgs[i]->label = atoi(token);
            }
            else
            {
                imgs[i]->img_data->entries[(j - 1) / 28][(j - 1) % 28] = atoi(token) / 256.0;
            }
            token = strtok(NULL, ",");
            j++;
        }
        img_print(imgs[i]);
        i++;
    }
    fclose(fp);
    return imgs;
}

void img_free(Img *img)
{
    matrix_free(img->img_data);
    free(img);
    img = NULL;
}

void imgs_free(Img **imgs, int n)
{
    for (int i = 0; i < n; i++)
    {
        img_free(imgs[i]);
    }
    free(imgs);
    imgs = NULL;
}
