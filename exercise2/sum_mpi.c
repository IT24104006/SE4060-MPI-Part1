#include <stdio.h>
#include <mpi.h>

#define N 10000000LL
#define REPS 50

int main(void)
{
    int rank, size;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    volatile long long zero = 0;   /* keeps the compiler from removing the repeats */

    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();

    long long start = (N * rank) / size + 1;
    long long end   = (N * (rank + 1)) / size;
    long long local = 0;
    for (int rep = 0; rep < REPS; rep++) {
        local = 0;
        for (long long i = start; i <= end; i++)
            local += i + zero;
    }

    long long total = 0;
    if (rank == 0) {
        long long part;
        total = local;
        for (int src = 1; src < size; src++) {
            MPI_Recv(&part, 1, MPI_LONG_LONG, src, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total += part;
        }
    } else {
        MPI_Send(&local, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double t1 = MPI_Wtime();

    if (rank == 0)
        printf("Processes: %d  Sum: %lld  Time: %f s\n", size, total, t1 - t0);

    MPI_Finalize();
    return 0;
}
