#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 9  // size of the matrix

// this function prints a (dim1, dim2) matrix of integers to the stdout
void print_matrix(int** A, int dim1, int dim2) {
    for (int i=0; i < dim1; ++i) {
        for (int j=0; j < dim2; ++j) {
            printf("%d", A[i][j]);
        }
        printf("\n");
    }
}

// this function allocates to the heap and returns a (dim1, dim2) matrix of integers
int** allocate_matrix(int dim1, int dim2) {
    int** A = (int**)malloc(dim1 * sizeof(int*));
    for (int i=0; i < dim1; ++i) {
        A[i] = (int*)malloc(dim2 * sizeof(int));
    }
    return A;
}

// this function deallocates a (dim1, *) matrix of integers
void deallocate_matrix(int** A, int dim1) {
    for (int i=0; i < dim1; ++i) {
        free(A[i]);
    }
    free(A);
}

int main(int argc, char* argv[]) {

    int rank = 0;
    int npes = 1;
    MPI_Request mpi_requests2[(N % npes == 0) ? N / npes : N / npes + 1];
    MPI_Request mpi_requests1[(N % npes == 0) ? N / npes : N / npes + 1];

    // intialization
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &npes);

    // local variables
    int local_N = N / npes;
    int rest = N % npes;
    if (rank < rest) ++local_N;  // distribute the rest if N % npes != 0

    // allocate matrix
    int** Mat = (int**)malloc(local_N * sizeof(int*));
    for (int i=0; i < local_N; ++i) {
        Mat[i] = (int*)malloc(N * sizeof(int));
    }

    // fill matrix with identity. FIX ALIGNMENT BUG
    for (int i=0; i < local_N; ++i) {
        for (int j=0; j < N; ++j) {
            if (rank != rest) {
                if ((i + (local_N * rank)) == j) {
                    Mat[i][j] = 1;
                }
                else {
                    Mat[i][j] = 0;
                }
            }
            else {
                if ((i + ((local_N + 1) * rank)) == j) {
                    Mat[i][j] = 1;
                }
                else {
                    Mat[i][j] = 0;
                }
            }
        }
    }

    // all processes send their distributed portion to 0 the root process 0
    if (rank != 0) {
        MPI_Send(&local_N, 1, MPI_INT, 0, 101, MPI_COMM_WORLD);
        for (int i=0; i < local_N; ++i) {
            MPI_Isend(Mat[i], N, MPI_INT, 0, 101, MPI_COMM_WORLD, &mpi_requests1[i]); // non-blocking send row-by-row
        }
    }
    else {  // process 0 orderly receives and prints to the stdout/write on binary file
        if (N < 10) {  // if N < 10 print on stdout
            print_matrix(Mat, local_N, N);
            deallocate_matrix(Mat, local_N);
            // for all the other processes: receive, allocate space, print, and deallocate
            for (int curr_rank=1; curr_rank < npes; ++curr_rank) {
                MPI_Recv(&local_N, 1, MPI_INT, curr_rank, 101, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                int** Mat = allocate_matrix(local_N, N);
                for (int i=0; i < local_N; ++i) {
                    MPI_Irecv(Mat[i], N, MPI_INT, curr_rank, 101, MPI_COMM_WORLD, &mpi_requests2[i]);  // non-blocking receive row-by-row
                }
                // print row-by-row, making sure the one under consideration has actually arrived
                for (int i=0; i < local_N; ++i) {
                    MPI_Wait(&mpi_requests2[i], MPI_STATUS_IGNORE);
                    for (int j=0; j < N; ++j) {
                        printf("%d", Mat[i][j]);
                    }
                    printf("\n");
                }
                deallocate_matrix(Mat, local_N);
            }
        }
	else {  // else write on binary file
            FILE* fp = fopen("distributed_matrix2.dat", "w"); // open a file in write mode
            fread(Mat, sizeof(int), local_N * N, fp);
            int local_size_buf = local_N * N * sizeof(int);
            fseek(fp, local_size_buf, SEEK_CUR);  // move the file pointer just after the currently written region
            deallocate_matrix(Mat, local_N);
            // do the same as the above for loop, but writing on file
            for (int curr_rank=1; curr_rank < npes; ++curr_rank) {
                MPI_Recv(&local_N, 1, MPI_INT, curr_rank, 101, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                int** Mat = allocate_matrix(local_N, N);
                for (int i=0; i < local_N; ++i) {
                    MPI_Irecv(Mat[i], N, MPI_INT, curr_rank, 101, MPI_COMM_WORLD, &mpi_requests2[i]);  // non-blocking receive row-by-row
                }
                // write row-by-row, making sure the one under consideration has actually arrived
                for (int i=0; i < local_N; ++i) {
                    MPI_Wait(&mpi_requests2[i], MPI_STATUS_IGNORE);
                    fread(Mat[i], sizeof(int), N, fp);
                    int local_size_buf = N * sizeof(int);
                    fseek(fp, local_size_buf, SEEK_CUR); // move the file pointer
                }
                deallocate_matrix(Mat, local_N);
            }
	    fclose(fp); // release resources
        }
    }

    // deallocate and finalize
    if (rank != 0) {
        for (int i=0; i < local_N; ++i) {
            MPI_Wait(&mpi_requests1[i], MPI_STATUS_IGNORE);  // make sure data have been sent before calling free
        }
        deallocate_matrix(Mat, local_N);
    }

    MPI_Finalize();

    return 0;
}
