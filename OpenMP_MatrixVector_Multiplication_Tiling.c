#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define TILE_SIZE 16

void matrixVectorMultTiled(int *matrix, int *vector, int *result, int rows, int cols) {
    int i, j, k;
    #pragma omp parallel for shared(matrix, vector, result) private(i, j, k)
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            result[i] += matrix[i * cols + j] * vector[j];
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 1) {
        printf("Usage: %s\n", argv[0]);
        return 1;
    }

    int matrix_size, vector_size;

    printf("Enter matrix size: ");
    scanf("%d", &matrix_size);
    printf("Enter vector size: ");
    scanf("%d", &vector_size);

    int *matrix = (int *)malloc(matrix_size * vector_size * sizeof(int));
    int *vector = (int *)malloc(vector_size * sizeof(int));
    int *result = (int *)malloc(matrix_size * sizeof(int));

    #pragma omp parallel for
    for (int i = 0; i < matrix_size * vector_size; ++i) {
        matrix[i] = rand() % 10; 
    }
    #pragma omp parallel for
    for (int i = 0; i < vector_size; ++i) {
        vector[i] = rand() % 10; 
    }
    double start_time = omp_get_wtime();
    matrixVectorMultTiled(matrix, vector, result, matrix_size, vector_size);
    double end_time = omp_get_wtime();

    printf("Resultant vector:\n");
    for (int i = 0; i < matrix_size; ++i) {
        printf("%d ", result[i]);
    }
    printf("\n");
    printf("Execution time: %lf seconds\n", end_time - start_time);
    
    free(matrix);
    free(vector);
    free(result);

    return 0;
}
