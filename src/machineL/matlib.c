#include "matlib.h"

float sigmoidf(float x)
{
    return 1.f / (1.f + expf(-x));
}

float rand_float(void)
{
    return (float)rand() / (float)RAND_MAX;
}

Mat mat_alloc(size_t rows, size_t cols)
{
    Mat m;
    m.rows = rows;
    m.cols = cols;
    m.stride = cols;
    m.es = malloc(sizeof(*m.es) * rows * cols);
    return m;
}

void mat_dot(Mat dst, Mat a, Mat b)
{
    size_t n = a.cols;
    for (size_t i = 0; i < dst.rows; ++i)
    {
        for (size_t j = 0; j < dst.cols; ++j)
        {
            MAT_AT(dst, i, j) = 0;
            for (size_t k = 0; k < n; ++k)
            {
                MAT_AT(dst, i, j) += MAT_AT(a, i, k) * MAT_AT(b, k, j);
            }
        }
    }
}

Mat mat_row(Mat m, size_t row)
{
    return (Mat){
        .rows = 1,
        .cols = m.cols,
        .stride = m.stride,
        .es = &MAT_AT(m, row, 0),
    };
}

void mat_copy(Mat dst, Mat src)
{
    for (size_t i = 0; i < dst.rows; ++i)
    {
        for (size_t j = 0; j < dst.cols; ++j)
        {
            MAT_AT(dst, i, j) = MAT_AT(src, i, j);
        }
    }
}

void mat_sum(Mat dst, Mat a)
{
    for (size_t i = 0; i < dst.rows; ++i)
    {
        for (size_t j = 0; j < dst.cols; ++j)
        {
            MAT_AT(dst, i, j) += MAT_AT(a, i, j);
        }
    }
}

void mat_app(Mat m, float (*func)(float))
{
    for (size_t i = 0; i < m.rows; ++i)
    {
        for (size_t j = 0; j < m.cols; ++j)
        {
            MAT_AT(m, i, j) = func(MAT_AT(m, i, j));
        }
    }
}

void mat_print(Mat m, const char *name, size_t padding)
{
    printf("%*s%s = [\n", (int)padding, "", name);
    for (size_t i = 0; i < m.rows; ++i)
    {
        printf("%*s    ", (int)padding, "");
        for (size_t j = 0; j < m.cols; ++j)
        {
            printf("%f ", MAT_AT(m, i, j));
        }
        printf("\n");
    }
    printf("%*s]\n", (int)padding, "");
}

void mat_fill(Mat m, float x)
{
    for (size_t i = 0; i < m.rows; ++i)
    {
        for (size_t j = 0; j < m.cols; ++j)
        {
            MAT_AT(m, i, j) = x;
        }
    }
}

void mat_rand(Mat m, float low, float high)
{
    for (size_t i = 0; i < m.rows; ++i)
    {
        for (size_t j = 0; j < m.cols; ++j)
        {
            MAT_AT(m, i, j) = rand_float() * (high - low) + low;
        }
    }
}

Mat read_matrix(char *filename)
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

    Mat m = mat_alloc(rows, cols);
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

void write_matrix(Mat m, char *filename)
{
    FILE *file = fopen(filename, "w");

    if (!file)
    {
        fprintf(stderr, "Error, couldn't create file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%zu %d\n", m.rows, m.cols);

    for (int i = 0; i < m.rows; ++i)
    {
        for (size_t j = 0; j < m.cols; ++j)
        {
            fprintf(file, "%.5f ", MAT_AT(m, i, j));
        }
        fprintf(file, "\n");
    }

    fclose(file);
}
