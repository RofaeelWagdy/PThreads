#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

// Structure to hold matrix data
typedef struct
{
    int rows;
    int cols;
    int **data;
} Matrix;

typedef struct
{
    int row;
    int col;
    Matrix *mat1;
    Matrix *mat2;
    Matrix *result;
} MulArguments;

// Function to allocate matrix
Matrix *createMatrix(int rows, int cols)
{
    Matrix *mat = malloc(sizeof(Matrix));

    mat->rows = rows;
    mat->cols = cols;

    // Allocate memory for the row pointers
    mat->data = (int **)malloc(rows * sizeof(int *));

    // Allocate memory for each row's columns
    for (int i = 0; i < rows; i++)
        mat->data[i] = (int *)malloc(cols * sizeof(int));

    return mat;
}

// Function to free matrix
void freeMatrix(Matrix *mat)
{
    for (int i = 0; i < mat->rows; i++)
        free(mat->data[i]);
    free(mat->data);
    free(mat);
}

// Function to read matrices from file
int readMatrices(Matrix **mat1, Matrix **mat2)
{
    FILE *file = fopen("input", "r");
    if (!file)
    {
        printf("Error opening file.\n");
        return 1;
    }

    int n_rows, n_columns;

    fscanf(file, "%d %d", &n_rows, &n_columns);
    *mat1 = createMatrix(n_rows, n_columns);
    for (int i = 0; i < n_rows; i++)
        for (int j = 0; j < n_columns; j++)
        {
            fscanf(file, "%d", &((*mat1)->data[i][j]));
            if (j < (*mat1)->cols - 1)
                fscanf(file, " ");
        }

    fscanf(file, "%d %d", &n_rows, &n_columns);
    *mat2 = createMatrix(n_rows, n_columns);
    for (int i = 0; i < n_rows; i++)
        for (int j = 0; j < n_columns; j++)
        {
            fscanf(file, "%d", &((*mat2)->data[i][j]));
            if (j < (*mat2)->cols - 1)
                fscanf(file, " ");
        }

    fclose(file);
    return 0;
}

// Function to print matrix
void printMatrix(Matrix *mat)
{
    for (int i = 0; i < mat->rows; i++)
    {
        for (int j = 0; j < mat->cols; j++)
        {
            printf("%d", mat->data[i][j]);
            if (j < mat->cols - 1)
                printf(" ");
        }
        printf("\n");
    }
}

// Method 1: One thread per element
void *computeElement(void *args)
{
    MulArguments *data = (MulArguments *)args;

    int sum = 0;
    for (int i = 0; i < data->mat1->cols; i++)
        sum += data->mat1->data[data->row][i] * data->mat2->data[i][data->col];

    data->result->data[data->row][data->col] = sum;
    free(data);
}

void multiplyMatricesPerElement(Matrix *mat1, Matrix *mat2, Matrix **result)
{
    *result = createMatrix(mat1->rows, mat2->cols);

    int num_threads = (*result)->rows * (*result)->cols;
    pthread_t threads[num_threads];
    int thread_index = 0;

    for (int i = 0; i < (*result)->rows; i++)
        for (int j = 0; j < (*result)->cols; j++)
        {
            MulArguments *args = (MulArguments *)malloc(sizeof(MulArguments));
            args->row = i;
            args->col = j;
            args->mat1 = mat1;
            args->mat2 = mat2;
            args->result = *result;

            pthread_create(&threads[thread_index++], NULL, computeElement, (void *)args);
        }

    for (int i = 0; i < num_threads; i++)
        pthread_join(threads[i], NULL);
}

// Method 2: One thread per row

void *computeRow(void *args)
{
    MulArguments *data = (MulArguments *)args;

    int sum = 0;
    for (int i = 0; i < data->result->cols; i++)
    {
        for (int j = 0; j < data->mat1->cols; j++)
        {
            sum += data->mat1->data[data->row][j] * data->mat2->data[j][i];
        }
        data->result->data[data->row][i] = sum;
        sum = 0;
    }
    free(data);
}

void multiplyMatricesPerRow(Matrix *mat1, Matrix *mat2, Matrix **result)
{
    *result = createMatrix(mat1->rows, mat2->cols);

    int num_threads = (*result)->rows;
    pthread_t threads[num_threads];
    int thread_index = 0;
    for (int i = 0; i < (*result)->rows; i++)
    {
        MulArguments *args = (MulArguments *)malloc(sizeof(MulArguments));
        args->row = i;
        args->mat1 = mat1;
        args->mat2 = mat2;
        args->result = *result;

        pthread_create(&threads[thread_index++], NULL, computeRow, (void *)args);
    }

    for (int i = 0; i < num_threads; i++)
        pthread_join(threads[i], NULL);
}

int main()
{

    Matrix *mat1, *mat2, *result1, *result2;
    struct timeval start, end;

    // Read matrices from file
    if (readMatrices(&mat1, &mat2) != 0)
    {
        fprintf(stderr, "Error reading matrices\n");
        return 1;
    }


    // Method 1: Per element
    gettimeofday(&start, NULL);
    multiplyMatricesPerElement(mat1, mat2, &result1);
    gettimeofday(&end, NULL);
    double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;

    printMatrix(result1);
    printf("END1 %f seconds\n\n", elapsed_time);


    // Method 2: Per row
    gettimeofday(&start, NULL);
    multiplyMatricesPerRow(mat1, mat2, &result2);
    gettimeofday(&end, NULL);
    elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;

    printMatrix(result2);
    printf("END2 %f seconds\n\n", elapsed_time);

    
    // Cleanup
    freeMatrix(mat1);
    freeMatrix(mat2);
    freeMatrix(result1);
    freeMatrix(result2);

    return 0;
}
