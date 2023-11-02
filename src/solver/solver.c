#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h> 
#include <string.h>

typedef unsigned char Sudoku[9][9];

FILE* parse_load_sudoku(FILE *file, Sudoku sudoku)
{
    size_t col = 0;
    size_t line = 0;
    char c = fgetc(file);
    while(c != EOF && (line < 9))
    {
        if(c != ' ' && c != '\n')
        {
            if(c == '.')
            {
                sudoku[line][col] = '0';
                col += 1;
            }
            
            else
            {
                sudoku[line][col] = c;
                col+=1;
            }
        
        }
        if(col % 9 == 0 && col != 0 )
        {
            line+= 1;
            col = 0;
        }
        
        c = fgetc(file);
    
    }
    
    return file;
    
}

void print_grid(Sudoku s)
{
    for (size_t i = 0; i < 9; i++)
    {
        for (size_t j = 0; j < 9; j++)
        {
            printf("%c ", s[i][j]);
        }
        printf("\n");
    }
}

int find_next_zero(Sudoku s, size_t *row, size_t *col)
{
     while (*row < 9 && *col < 9)
     {
       
        if (s[*row][*col] == '0')
        {
            return 1; // Found an empty cell.
        } 
        
        if(*col == 8)
        {
            *row += 1;
            *col = 0;
        }
        else
        {
            *col+=1;
        }
     }
    return 0; // No empty cells found; the puzzle is solved.
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
        if ((e == s[i][l] && l != j) ||( e == s[l][j] && l != i))
            return 0;
    }
    return 1;
}

int solve_sudoku(Sudoku s)
{
    size_t row =0;
    size_t col = 0;

    if (find_next_zero(s, &row, &col) == 0)
    {
        return EXIT_SUCCESS; // All cells filled, puzzle solved.
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

void write_sudoku(FILE *file, Sudoku s)
{
    for (size_t i = 0; i < 9; i++)
    {
        for (size_t j = 0; j < 9; j++)
        {
            fprintf(file, "%c", s[i][j]);
            fprintf(file, " ");
        }
        fprintf(file, "\n");
    }
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
    for(size_t i = 0 ; i < 9; i++)
    {
        for(size_t j =0; j< 9; j++)
	{
            if(s[i][j] != '0')
	    {
                if(check_legal(s, i, j, s[i][j]) == 0)
		{
                    return 1;
		}
	    }
	}
    }
    return 0;
}
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Invalid number of arguments\n");
        return 1;
    }
    FILE *file = fopen(argv[1], "r");
    Sudoku s;
    init_sudoku(s);
    file = parse_load_sudoku(file, s);
    if(check_grid(s) == 1)
    {
        printf("Le sudoku n'est pas resolvable.\n");
    }
    else  if(solve_sudoku(s) == EXIT_SUCCESS)
    {
	char* name = strcat(argv[1], ".result");
	FILE*out = fopen(name , "w");
    	write_sudoku(out, s);
	fclose(out);
    }
    else 
    {
        printf("Pas resolvable\n");
    }
    fclose(file);
    return 0;
}
