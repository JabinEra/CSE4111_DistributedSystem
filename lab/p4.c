

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int rank, size;
    int message_size = 1024;
    int iterations = 5;
    char *message;
    double start, end, total_time;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    message = malloc(message_size);

    for (int i = 0; i < message_size; i++)
    {
        message[i] = 'A';
    }

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    for (int i = 0; i < iterations; i++)
    {
        if (rank == 0)
        {
            MPI_Send(message, message_size, MPI_CHAR, 1, 0,MPI_COMM_WORLD);

            MPI_Recv(message, message_size, MPI_CHAR, 1, 0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        else
        {
            MPI_Recv(message, message_size, MPI_CHAR, 0, 0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            MPI_Send(message, message_size, MPI_CHAR, 0, 0,MPI_COMM_WORLD);
        }
    }

    end = MPI_Wtime();

    if (rank == 0)
    {
        total_time = end - start;

        printf("Message size          : %d bytes\n", message_size);
        printf("Iterations            : %d\n", iterations);
        printf("Total round trips     : %d\n", iterations);
        printf("Average round-trip    : %f seconds\n",total_time / iterations);
        printf("Average one-way       : %f seconds\n",total_time / iterations / 2);
    }

    free(message);
    MPI_Finalize();

    return 0;
}