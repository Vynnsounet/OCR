#include "matlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Matrix *alloc_matrix(int r, int c)
{
    Matrix *m = malloc(sizeof(Matrix));

    m->rows = r;
    m->columns = c;
    m->entries = (float *)calloc(sizeof(float), r * c);

    return m;
}

float randfloat(float a, float b)
{
    return (float)rand() / ((float)RAND_MAX / (b - a)) + a;
}

void print_matrix(Matrix *m)
{
    for (int i = 0; i < m->rows; ++i)
    {
        for (int j = 0; j < m->columns; ++j)
        {
            printf("%.5f ", MAT_AT(m, i, j));
        }
        printf("\n");
    }
}

void free_matrix(Matrix *m)
{
    free(m->entries);
    free(m);
}

// adds matrix a into matrix b
void add(Matrix *a, Matrix *b)
{
    if (a->rows == b->rows && a->columns == b->columns)
    {
        for (int i = 0; i < a->rows; ++i)
        {
            for (int j = 0; j < a->columns; ++j)
            {
                MAT_AT(b, i, j) += MAT_AT(a, i, j);
            }
        }
    }

    fprintf(stderr, "%s", "Matrices aren't the same size.\n");
    exit(EXIT_FAILURE);
}

void apply_func(Matrix *a, float (*func)(float))
{
    for (int i = 0; i < a->columns; ++i)
    {
        for (int j = 0; j < a->rows; ++j)
        {
            MAT_AT(a, i, j) = func(MAT_AT(a, i, j));
        }
    }
}

void rand_mat(Matrix *a, float l, float h)
{
    for (int i = 0; i < a->rows; ++i)
    {
        for (int j = 0; j < a->columns; ++j)
        {
            MAT_AT(a, i, j) = randfloat(l, h);
        }
    }
}

void xmul(Matrix *a, float x)
{
    for (int i = 0; i < a->columns; ++i)
    {
        for (int j = 0; i < a->rows; ++j)
        {
            MAT_AT(a, i, j) *= x;
        }
    }
}

Matrix *mul(Matrix *a, Matrix *b)
{
    if (a->columns != b->rows)
    {
        fprintf(stderr, "%s", "Cannot multiply these two matrices.\n");
        exit(EXIT_FAILURE);
    }

    Matrix *m = alloc_matrix(a->rows, b->columns);
    for (int i = 0; i < m->rows; ++i)
    {
        for (int j = 0; j < m->columns; ++j)
        {
            for (int k = 0; k < a->columns; ++k)
            {
                MAT_AT(m, i, j) += MAT_AT(a, i, k) * MAT_AT(b, k, j);
            }
        }
    }
    return m;
}

Matrix *read_matrix(char *filename)
{
    FILE *file = fopen(filename, "r");

    if (!file)
    {
        fprintf(stderr, "Error, couldn't open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    int rows, cols;
    char *buffer;
    size_t len = 0;
    ssize_t read;

    // getting matrix size
    getline(&buffer, &len, file);
    sscanf(buffer, "%d %d", &rows, &cols);

    Matrix *m = alloc_matrix(rows, cols);
    int curr_line = 0;
    while (getline(&buffer, &len, file) != -1)
    {
        for (ssize_t i = 0; i < cols; i++)
        {
            sscanf(buffer, "%f", &MAT_AT(m, curr_line, i));
            while (*buffer != ' ' && *buffer != '\0')
            {
                buffer++; // move the string pointer forward until a space or null terminator is encountered
            }
            if (*buffer == '\0')
            {
                break; // exit the loop if we've reached the end of the string
            }
            buffer++; // move past the space
        }
        curr_line++;
    }

    fclose(file);
    return m;
}

void write_matrix(Matrix *m, char *filename)
{
    FILE *file = fopen(filename, "w");

    if (!file)
    {
        fprintf(stderr, "Error, couldn't create file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%d %d\n", m->rows, m->columns);
    for (int i = 0; i < m->rows; ++i)
    {
        for (size_t j = 0; j < m->columns; ++j)
        {
            fprintf(file, "%.5f ", MAT_AT(m, i, j));
        }
        fprintf(file, "\n");
    }

    fclose(file);
}
