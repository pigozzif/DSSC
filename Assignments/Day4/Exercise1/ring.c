#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 5  // dimesion of the vectors
#define PROC_DOWN(x, npes) (x - 1 + npes) % npes  // the rank of the process to the left
#define PROC_UP(x, npes) (x + 1) % npes  // the rank of the process to the right


int main(int argc, char* argv[]) {

    int rank = 0;
    int npes = 1;
    MPI_Request request;  // request handler for the non-blocking send

    // initialization
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &npes);

    // local buffers
    int* X = (int*)malloc(N * sizeof(int));
    int* sum = (int*)malloc(N * sizeof(int));

    // init local buffers
    // X = me
    // sum = X
    for (int i=0; i < N; ++i) {
        X[i] = rank;
        sum[i] = X[i];
    }

    // for all processes, receive from the left and locally accumulate
    // then send to the right
    if (rank != 0) {
        MPI_Recv(sum, 1, MPI_INT, PROC_DOWN(rank, npes), 101, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (int i=0; i < N; ++i) sum[i] += X[i];  // accumulate
        printf("Process %d received token and summed to %d from process %d\n", rank, sum[0], PROC_DOWN(rank, npes));
    }

    MPI_Isend(sum, 1, MPI_INT, PROC_UP(rank, npes), 101, MPI_COMM_WORLD, &request);  // non-blocking send

    if (rank == 0) {
        MPI_Recv(sum, 1, MPI_INT, PROC_DOWN(rank, npes), 101, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (int i=0; i < N; ++i) sum[i] += X[i];  // accumulate
        printf("Process %d received token and summed to %d from process %d\n", rank, sum[0], PROC_DOWN(rank, npes));
    }

    // deallocation and finalization
    free(X);
    free(sum);
    MPI_Finalize();

    return 0;
}
