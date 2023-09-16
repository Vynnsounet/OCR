#ifndef MATLIB_H
#define MATLIB_H
#include <time.h>

#define MAT_AT(m, i, j) (m)->entries[(i) * (m)->columns + (j)]
typedef struct
{
    float *entries;
    int rows;
    int columns;

} Matrix;

float randfloat(float a, float b);

void rand_mat(Matrix *a, float l, float h);

Matrix *alloc_matrix(int r, int c);

void print_matrix(Matrix *m);

void free_matrix(Matrix *m);

void add(Matrix *a, Matrix *b);

void apply_func(Matrix *a, float (*func)(float));

void xmul(Matrix *a, float x);

Matrix *mul(Matrix *a, Matrix *b);

Matrix *read_matrix(char *filename);

void write_matrix(Matrix *m, char *filename);

#endif // !MATLIB_H
