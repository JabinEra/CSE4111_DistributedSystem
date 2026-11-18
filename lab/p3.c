
#include <mpi.h>
#include <stdio.h>


int main(int argc, char *argv[]) {
    int rank, size, received;
    int value = 100;
    double start, end;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int next = (rank + 1) % size;
    int prev = (rank - 1 + size) % size;

    //value = rank == 0 ? initial : rank * 10;

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    MPI_Sendrecv(&value, 1, MPI_INT, next, 0,
                 &received, 1, MPI_INT, prev, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    end = MPI_Wtime();

    printf("P%d sends %d to P%d\n", rank, value, next);
    printf("P%d receives %d from P%d\n",
           rank, received, prev);

    double elapsed = end - start, max_elapsed;
    MPI_Reduce(&elapsed, &max_elapsed, 1, MPI_DOUBLE,
               MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Ring communication completed.\n");
        printf("Maximum communication time = %.9f seconds\n",
               max_elapsed);
    }

    MPI_Finalize();
    return 0;
}


// #include <mpi.h>
// #include <stdio.h>

// int main(int argc, char **argv)
// {
//     int rank, size;
//     int value = 100;
//     int received;
//     double start, end;

//     MPI_Init(&argc, &argv);
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//     MPI_Comm_size(MPI_COMM_WORLD, &size);

//     int next = (rank + 1) % size;
//     int prev = (rank - 1 + size) % size;

//     if (rank == 0)
//         start = MPI_Wtime();

//     MPI_Bcast(&value, 1, MPI_INT, 0, MPI_COMM_WORLD);

//     MPI_Sendrecv(&value, 1, MPI_INT, next, 0,
//                  &received, 1, MPI_INT, prev, 0,
//                  MPI_COMM_WORLD, MPI_STATUS_IGNORE);

//     printf("P%d receives %d from P%d\n", rank, received, prev);

//     MPI_Barrier(MPI_COMM_WORLD);

//     if (rank == 0) {
//         end = MPI_Wtime();
//         printf("Ring communication completed\n");
//         printf("Communication time = %f seconds\n", end - start);
//     }

//     MPI_Finalize();
//     return 0;
// }