#include <stdio.h>
#include <mpi.h>

int main(void)
{
    int rank, x = 30, y = 0;
    MPI_Status status;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 1) {
        printf("Rank 1: sending to rank 3\n");
        fflush(stdout);
        MPI_Ssend(&x, 1, MPI_INT, 3, 0, MPI_COMM_WORLD);
        printf("Rank 1: send completed\n");
    } else if (rank == 3) {
        printf("Rank 3: waiting for a message from rank 2\n");
        fflush(stdout);
        MPI_Recv(&y, 1, MPI_INT, 2, 0, MPI_COMM_WORLD, &status);
        printf("Rank 3: received %d\n", y);
    }

    MPI_Finalize();
    return 0;
}
