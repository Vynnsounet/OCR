#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned char Sudoku[9][9];

void parse_sudoku_entry(char *entry, unsigned char sudoku_line[9])
{
    size_t arr_index = 0;
    for (size_t i = 0; entry[i] != 0; ++i)
    {
        if (entry[i] != ' ')
        {
            sudoku_line[arr_index] = entry[i] == '.' ? 0 : entry[i] - '0';
            arr_index++;
        }
    }
}

FILE *load_sudoku(FILE *file, Sudoku s)
{
    char *buffer;
    int currline = 0;
    size_t len = 0;
    while (getline(&buffer, &len, file))
    {
        if (*buffer == '\n')
            break;
        parse_sudoku_entry(buffer, s[currline]);
        currline++;
    }
    return file;
}

void print_grid(Sudoku s)
{
    for (size_t i = 0; i < 9; i++)
    {
        for (size_t j = 0; j < 9; j++)
        {
            printf("%d ", s[i][j]);
        }
        printf("\n");
    }
}

int find_next_zero(Sudoku s, size_t *row, size_t *col)
{
    for (*row = 0; *row < 9; (*row)++)
    {
        for (*col = 0; *col < 9; (*col)++)
        {
            if (s[*row][*col] == 0)
            {
                return 1; // Found an empty cell.
            }
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
            if (e == s[ii][jj])
                return 1;
        }
    }
    return 0;
}

int check_legal(Sudoku s, size_t i, size_t j, unsigned char e)
{
    if (is_in_square(s, i, j, e))
        return 0;

    for (size_t l = 0; l < 9; l++)
    {
        if (e == s[i][l] || e == s[l][j])
            return 0;
    }
    return 1;
}

int solve_sudoku(Sudoku s)
{
    size_t row, col;

    if (!find_next_zero(s, &row, &col))
    {
        return 1; // All cells filled, puzzle solved.
    }

    for (unsigned char num = 1; num <= 9; num++)
    {
        if (check_legal(s, row, col, num))
        {
            s[row][col] = num;

            if (solve_sudoku(s))
            {
                return 1;
            }

            s[row][col] = 0;
        }
    }

    return 0;
}

void write_sudoku(FILE *file, Sudoku s)
{
    for (size_t i = 0; i < 9; i++)
    {
        for (size_t j = 0; j < 9; j++)
        {
            fprintf(file, "%d", s[i][j]);
            if (!j % 3)
                fprintf(file, " ");
        }
        fprintf(file, "\n");
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Invalid number of arguments\n");
        return 1;
    }
    FILE *file = fopen(argv[1], "r");
    FILE *out = fopen("sudoku_solved.txt", "w");
    Sudoku s;
    while (file)
    {
        file = load_sudoku(file, s);
        if (solve_sudoku(s))
            write_sudoku(out, s);
        else
            printf("Couldn't solve sudoku");
    }
    fclose(file);
    fclose(out);
    return 0;
}
