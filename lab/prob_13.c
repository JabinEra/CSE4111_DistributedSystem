#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 3) {
        if (rank == 0) printf("Run with 3 processes for the sample.\n");
        MPI_Finalize();
        return 0;
    }

    const int rows = 5, cols = 3;
    int matrix[5][3] = {
        {1,2,3}, {4,5,6}, {7,8,9},
        {10,11,12}, {13,14,15}
    };

    int sendcounts[3] = {6,6,3};
    int displs[3] = {0,6,12};

    int local_count = sendcounts[rank];
    int *local = malloc(local_count * sizeof(int));

    MPI_Scatterv(&matrix[0][0], sendcounts, displs, MPI_INT,
                 local, local_count, MPI_INT,
                 0, MPI_COMM_WORLD);

    for (int i = 0; i < local_count; i++)
        local[i] *= 2;

    int result[15];

    MPI_Gatherv(local, local_count, MPI_INT,
                result, sendcounts, displs, MPI_INT,
                0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Final matrix:\n");
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++)
                printf("%d ", result[i*cols+j]);
            printf("\n");
        }
    }

    free(local);
    MPI_Finalize();
    return 0;
}
