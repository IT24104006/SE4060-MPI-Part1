#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 10000000LL

int main(void)
{
    int rank, size;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();

    long long start = (N * rank) / size;
    long long end   = (N * (rank + 1)) / size;
    unsigned int seed = 12345u + (unsigned int)rank;
    long long local = 0;
    for (long long i = start; i < end; i++) {
        double x = (double)rand_r(&seed) / RAND_MAX;
        double y = (double)rand_r(&seed) / RAND_MAX;
        if (x * x + y * y <= 1.0)
            local++;
    }

    long long hits = 0;
    int *order = malloc(size * sizeof(int));
    if (rank == 0) {
        long long part;
        MPI_Status status;
        hits = local;
        for (int k = 1; k < size; k++) {
            MPI_Recv(&part, 1, MPI_LONG_LONG, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            order[k - 1] = status.MPI_SOURCE;
            hits += part;
        }
    } else {
        MPI_Send(&local, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double t1 = MPI_Wtime();

    if (rank == 0) {
        printf("Arrival order:");
        for (int k = 0; k < size - 1; k++)
            printf(" %d", order[k]);
        printf("\n");
        printf("Processes: %d  Pi: %.6f  Time: %f s\n", size, 4.0 * (double)hits / (double)N, t1 - t0);
    }

    free(order);
    MPI_Finalize();
    return 0;
}
