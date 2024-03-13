#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define TILE_SIZE 16

void matrixVectorMultTiled(int *matrix, int *vector, int *result, int rows, int cols, int rank, int size) {
    int chunk_size = rows / size;
    int *local_matrix = (int *)malloc(chunk_size * cols * sizeof(int)); 
    int *local_result = (int *)malloc(chunk_size * sizeof(int));

    MPI_Scatter(matrix, chunk_size * cols, MPI_INT, local_matrix, chunk_size * cols, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(vector, cols, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < chunk_size; ++i) {
        local_result[i] = 0;
        for (int j = 0; j < cols; ++j) {
            local_result[i] += local_matrix[i * cols + j] * vector[j];
        }
    }

    MPI_Gather(local_result, chunk_size, MPI_INT, result, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    free(local_matrix);
    free(local_result);
}

int main(int argc, char *argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 1) {
        if (rank == 0)
            printf("Usage: %s\n", argv[0]);
        MPI_Finalize();
        return 1;
    }

    int matrix_size, vector_size;

    if (rank == 0) {
        printf("Enter matrix size: ");
        scanf("%d", &matrix_size);
        printf("Enter vector size: ");
        scanf("%d", &vector_size);

        if (matrix_size % size != 0) {
            printf("Matrix size must be divisible by number of processes.\n");
            MPI_Finalize();
            return 1;
        }
    }

    MPI_Bcast(&matrix_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&vector_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int *matrix, *vector, *result;
    if (rank == 0) {
        matrix = (int *)malloc(matrix_size * vector_size * sizeof(int));
        vector = (int *)malloc(vector_size * sizeof(int));
        result = (int *)malloc(matrix_size * sizeof(int));
        for (int i = 0; i < matrix_size * vector_size; ++i) {
            matrix[i] = rand() % 10; 
        }
        for (int i = 0; i < vector_size; ++i) {
            vector[i] = rand() % 10; 
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double start_time = MPI_Wtime();

    if (rank == 0) {
        matrixVectorMultTiled(matrix, vector, result, matrix_size, vector_size, rank, size);
        printf("Resultant vector:\n");
        for (int i = 0; i < matrix_size; ++i) {
            printf("%d ", result[i]);
        }
        printf("\n");
        free(matrix);
        free(vector);
        free(result);
    } else {
        matrix = (int *)malloc((matrix_size / size) * vector_size * sizeof(int));
        vector = (int *)malloc(vector_size * sizeof(int));
        result = (int *)malloc((matrix_size / size) * sizeof(int));

        matrixVectorMultTiled(matrix, vector, result, matrix_size / size, vector_size, rank, size);

        free(matrix);
        free(vector);
        free(result);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double end_time = MPI_Wtime();
    if (rank == 0) {
        printf("Time taken: %lf seconds\n", end_time - start_time);
    }

    MPI_Finalize();

    return 0;
}
