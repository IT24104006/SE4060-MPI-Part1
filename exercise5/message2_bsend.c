#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(void)
{
    int rank;
    MPI_Status status;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    char name[MPI_MAX_PROCESSOR_NAME];
    int len;
    MPI_Get_processor_name(name, &len);
    int x[10], y[10];

    if (rank == 1) {
        for (int r = 0; r < 10; r++)
            x[r] = 10 * r;

        int bufsize = 10 * sizeof(int) + MPI_BSEND_OVERHEAD;
        void *buf = malloc(bufsize);
        MPI_Buffer_attach(buf, bufsize);

        printf("Sending message to rank 3 from rank 1 using MPI_Bsend\n");
        MPI_Bsend(x, 10, MPI_INT, 3, 0, MPI_COMM_WORLD);

        /* overwrite the send array straight away */
        for (int r = 0; r < 10; r++)
            x[r] = -1;
        printf("Rank 1: x has been overwritten with -1 after MPI_Bsend\n");

        MPI_Buffer_detach(&buf, &bufsize);   /* waits until the buffered message is delivered */
        free(buf);
    }
    else if (rank == 3) {
        MPI_Recv(y, 10, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
        printf("Rank 3: received array y:");
        for (int r = 0; r < 10; r++)
            printf(" %d", y[r]);
        printf("\n");
    }
    else
        printf("Just a normal process From rank %d machine %s\n", rank, name);

    MPI_Finalize();
    return 0;
}
