#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>

typedef unsigned char Sudoku[9][9];

#define TEXTURE_AT(si,i,j) (si)->textures[i * 9 + j]
#define RECT_AT(si,i,j) (si)->rectangles[i * 9 + j]

struct SudokuImage
{
    SDL_Rect *rectangles;
    SDL_Texture **textures;
};

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT (WINDOW_WIDTH)

SDL_Surface *SurfaceFromRenderer(SDL_Renderer *renderer)
{
    int w,h;
    SDL_GetRendererOutputSize(renderer,&w,&h);
    SDL_Surface *image = SDL_CreateRGBSurface(0, w, h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, image->pixels, image->pitch);
    return image;
}

void DrawBorders(SDL_Renderer *renderer)
{
    SDL_RenderDrawLine(renderer,1,1,WINDOW_WIDTH - 1,1);
    SDL_RenderDrawLine(renderer,WINDOW_WIDTH - 1,1,WINDOW_WIDTH - 1,WINDOW_HEIGHT - 1);
    SDL_RenderDrawLine(renderer,WINDOW_WIDTH - 1,WINDOW_HEIGHT - 1,1,WINDOW_HEIGHT - 1);
    SDL_RenderDrawLine(renderer,1,WINDOW_HEIGHT - 1,1,1);

    SDL_RenderDrawLine(renderer,0,0,WINDOW_WIDTH,0);
    SDL_RenderDrawLine(renderer,WINDOW_WIDTH,0,WINDOW_WIDTH,WINDOW_HEIGHT);
    SDL_RenderDrawLine(renderer,WINDOW_WIDTH,WINDOW_HEIGHT,0,WINDOW_HEIGHT);
    SDL_RenderDrawLine(renderer,0,WINDOW_HEIGHT,0,0);
}

void RenderGrid(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    DrawBorders(renderer);
    int step = WINDOW_WIDTH / 9;
    for(int i = step; i < WINDOW_WIDTH; i += step)
    {
        SDL_RenderDrawLine(renderer,i,0,i,WINDOW_HEIGHT);
        SDL_RenderDrawLine(renderer,i + 1,0,i + 1,WINDOW_HEIGHT);
        if(i%3 == 0)
        {
            SDL_RenderDrawLine(renderer,i + 2,0,i + 2,WINDOW_HEIGHT);
            SDL_RenderDrawLine(renderer,i + 3,0,i + + 3,WINDOW_HEIGHT);
            SDL_RenderDrawLine(renderer,i + 4,0,i + + 4,WINDOW_HEIGHT);
            SDL_RenderDrawLine(renderer,i + 5,0,i + + 5,WINDOW_HEIGHT);
        }
    }
    
    for(int i = step; i < WINDOW_HEIGHT; i += step)
    {
        SDL_RenderDrawLine(renderer,0,i,WINDOW_WIDTH,i);
        SDL_RenderDrawLine(renderer,0,i + 1,WINDOW_WIDTH,i + 1);

        if(i%3 == 0)
        {
            SDL_RenderDrawLine(renderer,0,i + 2,WINDOW_WIDTH,i + 2);
            SDL_RenderDrawLine(renderer,0,i + 3,WINDOW_WIDTH,i + 3);
            SDL_RenderDrawLine(renderer,0,i + 4,WINDOW_WIDTH,i + 4);
            SDL_RenderDrawLine(renderer,0,i + 5,WINDOW_WIDTH,i + 5);
        }
    }
}

/*
- x, y: upper left corner.
- texture, rect: outputs.
*/

void InitSudokuImage(SDL_Renderer *renderer,struct SudokuImage *si,TTF_Font *font,Sudoku s_init)
{
    si->rectangles = malloc(sizeof(SDL_Rect) * 81);
    si->textures = malloc(sizeof(SDL_Texture*) * 81);
    SDL_Surface *surface;
    SDL_Color textColor = {0, 0, 0, 0};
    SDL_Rect *rect;
    char str[2] = {0};
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            if(s_init[i][j] == '0')
                textColor.r = 255;
            str[0] = s_init[i][j];
            
            rect = &RECT_AT(si,i,j); 
            rect->x = i * 100 + 20;
            rect->y = j * 100;
            rect->w = 75;
            rect->h = 100;
            surface = TTF_RenderText_Solid(font, str, textColor);
            TEXTURE_AT(si,i,j) = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
            textColor.r = 0;
        }
    }


}

FILE *parse_load_sudoku(FILE *file, Sudoku sudoku)
{
    size_t col = 0, line = 0;
    char c;
    do
    {
        c = fgetc(file);
        if (c != ' ' && c != '\n')
        {
            if (c == '.')
            {
                sudoku[line][col] = '0';
                col += 1;
            }

            else
            {
                sudoku[line][col] = c;
                col += 1;
            }
        }
        if (col % 9 == 0 && col != 0)
        {
            line += 1;
            col = 0;
        }

    } while (c != EOF && (line < 9));

    return file;
}

int find_next_zero(Sudoku s, size_t *row, size_t *col)
{
    while (*row < 9 && *col < 9)
    {

        if (s[*row][*col] == '0')
        {
            return 1;
        }

        if (*col == 8)
        {
            *row += 1;
            *col = 0;
        }
        else
        {
            *col += 1;
        }
    }
    return 0;
}

int is_in_square(Sudoku s, size_t i, size_t j, unsigned char e)
{
    size_t maxR = 3 * (i / 3);
    size_t maxC = 3 * (j / 3);
    for (size_t ii = maxR; ii < maxR + 3; ii++)
    {
        for (size_t jj = maxC; jj < maxC + 3; jj++)
        {
            if (e == s[ii][jj] && (ii != i && jj != j))
                return 1;
        }
    }
    return 0;
}

int check_legal(Sudoku s, size_t i, size_t j, unsigned char e)
{
    if (is_in_square(s, i, j, e) == 1)
        return 0;

    for (size_t l = 0; l < 9; l++)
    {
        if ((e == s[i][l] && l != j) || (e == s[l][j] && l != i))
            return 0;
    }
    return 1;
}

void init_sudoku(Sudoku s)
{
    for (size_t i = 0; i < 9; i++)
    {
        for (size_t j = 0; j < 9; j++)
        {
            s[i][j] = '0';
        }
    }
}

int check_grid(Sudoku s)
{
    for (size_t i = 0; i < 9; i++)
    {
        for (size_t j = 0; j < 9; j++)
        {
            if (s[i][j] != '0')
            {
                if (check_legal(s, i, j, s[i][j]) == 0)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}
int solve_sudoku(Sudoku s)
{
    size_t row = 0;
    size_t col = 0;

    if (find_next_zero(s, &row, &col) == 0)
    {
        return EXIT_SUCCESS;
    }

    for (unsigned char num = '1'; num <= '9'; num++)
    {
        if (check_legal(s, row, col, num) == 1)
        {
            s[row][col] = num;

            if (solve_sudoku(s) == EXIT_SUCCESS)
            {
                return EXIT_SUCCESS;
            }
            s[row][col] = '0';
        }
    }

    return EXIT_FAILURE;
}

int main(int argc ,char **argv) 
{
    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;
    struct SudokuImage si; 


    /* Inint TTF. */
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_WIDTH, 0, &window, &renderer);
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("fonts/arial_bold.ttf", 100);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDL_RenderClear(renderer);

    if (argc != 2)
    {
        fprintf(stderr, "Invalid number of arguments\n");
        return 1;
    }
    FILE *file = fopen(argv[1], "r");
    Sudoku s;
    init_sudoku(s);
    file = parse_load_sudoku(file, s);
    solve_sudoku(s);
    fclose(file);
    
    InitSudokuImage(renderer,&si,font,s);
    RenderGrid(renderer);
    for(int i = 0; i < 81; i++)
    {

        SDL_RenderCopy(renderer, si.textures[i], NULL, &si.rectangles[i]);
        SDL_DestroyTexture(si.textures[i]);
    }
    free(si.textures);
    free(si.rectangles);
    SDL_RenderPresent(renderer);

    SDL_Surface *image = SurfaceFromRenderer(renderer);
    SDL_SaveBMP(image,"my_image.bmp");
    SDL_FreeSurface(image);

    /* Deinit TTF. */
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
