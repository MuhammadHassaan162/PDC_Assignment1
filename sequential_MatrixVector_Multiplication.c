#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void matrixVectorMult(int *matrix, int *vector, int *result, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        result[i] = 0;
        for (int j = 0; j < cols; ++j) {
            result[i] += matrix[i * cols + j] * vector[j];
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <matrix_size> <vector_size>\n", argv[0]);
        return 1;
    }

    int matrix_size, vector_size;
    printf("Enter the size of the matrix: ");
    scanf("%d", &matrix_size);
    printf("Enter the size of the vector: ");
    scanf("%d", &vector_size);

    int *matrix = (int *)malloc(matrix_size * vector_size * sizeof(int));
    int *vector = (int *)malloc(vector_size * sizeof(int));
    int *result = (int *)malloc(matrix_size * sizeof(int));

    srand(time(NULL));
    for (int i = 0; i < matrix_size * vector_size; ++i) {
        matrix[i] = rand() % 10; 
    }
    for (int i = 0; i < vector_size; ++i) {
        vector[i] = rand() % 10; 
    }

    clock_t start_time = clock(); 


    matrixVectorMult(matrix, vector, result, matrix_size, vector_size);

    clock_t end_time = clock(); 
    double execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("Resultant vector:\n");
    for (int i = 0; i < matrix_size; ++i) {
        printf("%d ", result[i]);
    }
    printf("\n");

    printf("Execution time: %.6f seconds\n", execution_time);
    free(matrix);
    free(vector);
    free(result);

    return 0;
}
