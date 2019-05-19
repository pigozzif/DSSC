#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 10  // size of the matrix
#define INDEX(x, y) x * N + y  // convert 2-d coordinates into 1-d, since matrices are manipulated as 1-d arrays

// this function swaps two pointers, whose addresses have been passed as arguments
void swap(int** a, int** b) {
    int* temp = *a;
    *a = *b;
    *b = temp;
}

// this function prints an array as a (dim1, dim2) matrix of integers to the stdout
void print_matrix(int* A, int dim1, int dim2) {
    for (int i=0; i < dim1; ++i) {
        for (int j=0; j < dim2; ++j) {
            printf("%d", A[INDEX(i, j)]);
        }
        printf("\n");
    }
}


int main(int argc, char* argv[]) {

    int rank = 0;
    int npes = 1;
    MPI_Request request;

    // intialization
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &npes);

    // local variables
    int local_N = N / npes;
    int rest = N % npes;
    if (rank < rest) ++local_N;  // distribute the rest if N % npes != 0

    // allocate matrix
    int* Mat = (int*)malloc(local_N * N * sizeof(int));

    // fill matrix with identity
    for (int i=0; i < local_N; ++i) {
        int i_global = i + (rank * local_N);
        for (int j=0; j < N; ++j) {
            if (i_global == j) {
                Mat[INDEX(i, j)] = 1;
            }
            else {
                Mat[INDEX(i, j)] = 0;
            }
        }
    }

    // all processes send their distributed portion to 0 the root process
    if (rank != 0) {
        MPI_Send(Mat, local_N * N, MPI_INT, 0, 101, MPI_COMM_WORLD);
    }
    else {  // process 0 orderly receives and prints to the stdout/write on binary file
        // allocate extra array to store the receiving portion. Needed to implement non-blocking version
        int* recv_buf = (int*)malloc(local_N * N * sizeof(int));  // the correctness follows from how we distributed the rest
        if (N < 10) {  // if N < 10 print on stdout
            // for all the other processes: non-blocking receive, compute (print), wait, swap pointers
            for (int curr_rank=1; curr_rank < npes; ++curr_rank) {
                MPI_Irecv(recv_buf, local_N * N, MPI_INT, curr_rank, 101, MPI_COMM_WORLD, &request);
                print_matrix(Mat, local_N, N);
                MPI_Wait(&request, MPI_STATUS_IGNORE);  // otherwise we cannot be sure we can swap the pointers
                swap(&recv_buf, &Mat);
            }
            print_matrix(Mat, local_N, N);  // we were missing the last one
        }
	else {  // else write on binary file
            FILE* fp = fopen("distributed_matrix.dat", "w"); // open a file in write mode
            int local_size_buf = local_N * N * sizeof(int);  // size of the portions to write
            // do the same as the above for loop, but writing on file
            for (int curr_rank=1; curr_rank < npes; ++curr_rank) {
                MPI_Irecv(recv_buf, local_N * N, MPI_INT, curr_rank, 101, MPI_COMM_WORLD, &request);
                fread(Mat, sizeof(int), local_N * N, fp);  // write
                fseek(fp, local_size_buf, SEEK_CUR);  // move the file pointer from the current position
                /** WRITE ON FILE **/
                MPI_Wait(&request, MPI_STATUS_IGNORE);  // otherwise we cannot be sure we can swap the pointers
                swap(&recv_buf, &Mat);
            }
	    fclose(fp); // release resources
        }
        // deallocated here because only 0 does it
        free(recv_buf);
    }

    // deallocate and finalize
    free(Mat);

    MPI_Finalize();

    return 0;
}
