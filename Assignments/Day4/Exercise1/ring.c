#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#ifndef VECTOR
#define N 1  // scalar message
#else
#define N 5  // dimension of the vector
#endif

#define PROC_LEFT(x, npes) (x - 1 + npes) % npes  // the rank of the process to the left
#define PROC_RIGHT(x, npes) (x + 1) % npes  // the rank of the process to the right

/* Main function */

int main(int argc, char* argv[]) {

    int rank = 0;
    int npes = 1;
    MPI_Request request;  // request handler for the non-blocking send

    // initialization
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &npes);

    #ifndef VECTOR
    X = rank;
    sum = 0;
    #else
    // allocate local buffers
    int* X = (int*)malloc(N * sizeof(int));
    int* sum = (int*)malloc(N * sizeof(int));

    // init local buffers
    // X = me
    // sum = X
    for (int i=0; i < N; ++i) {
        X[i] = rank;
        sum[i] = 0;
    }
    #endif
    
    // iterate over all the processes:
    for (int iter=0; iter < npes; ++iter) {
        // 1. send X to the right
        MPI_Isend(X, N, MPI_INT, PROC_RIGHT(rank, npes), 101, MPI_COMM_WORLD, &request);  // non-blocking send
        // 2. perform computation: sum = sum + X
        #ifndef VECTOR
        sum += X;
        #else
        for (int i=0; i < N; ++i) {
            sum[i] += X[i];
        }
        #endif
        // 3. recv X from left
        MPI_Wait(&request, MPI_STATUS_IGNORE);
        MPI_Recv(X, N, MPI_INT, PROC_LEFT(rank, npes), 101, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    // take care of last computation
    #ifndef VECTOR
    sum += X;
    #else
    for (int i=0; i < N; ++i) {
            sum[i] += X[i];
        }
    #endif
    
    // check result
    printf("Process %d has the following buffer:\n", rank);
    #ifndef VECTOR
    printf("%d\n", sum);
    #else
    for (int i=0; i < N; ++i) {
        printf("%d\t", sum[i]);
    }
    printf("\n");
    #endif
    
    // deallocate and finalize
    free(X);
    free(sum);
    
    MPI_Finalize();

    return 0;
}
