#include <stdio.h>
#include <stdlib.h>

#define MAT_AT(m, i, j) (m)->entries[(i) * (m)->rows + (j)]

typedef struct
{
    int rows;
    int columns;
    float *entries;

} Matrix;

Matrix *alloc_matrix(int l, int h)
{
    Matrix *m = malloc(sizeof(Matrix));

    m->rows = h;
    m->columns = l;
    m->entries = (float *)calloc(sizeof(float), l * h);

    return m;
}

void free_matrix(Matrix *m)
{
    free(m->entries);
    free(m);
}

// adds matrix a into matrix b
void add_matrix(Matrix *a, Matrix *b)
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
        for (int j = 0; i < a->rows; ++j)
        {
            MAT_AT(a, i, j) = func(MAT_AT(a, i, j));
        }
    }
}

void r_mul(Matrix *a, float x)
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

// TODO
//  read matrix from file (return a new matrix)
//  write a matrix to a file (return nothing)
