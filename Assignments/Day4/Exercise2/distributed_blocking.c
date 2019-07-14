#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 9  // size of the matrix
#define INDEX(x, y) x * N + y  // convert 2-d coordinates into 1-d, since matrices are manipulated as 1-d arrays


/* this function prints an array as a (dim1, dim2) matrix of integers to the stdout */

void print_matrix(int* A, int dim1, int dim2) {
    for (int i=0; i < dim1; ++i) {
        for (int j=0; j < dim2; ++j) {
            printf("%d", A[INDEX(i, j)]);
        }
        printf("\n");
    }
}

/* this function writes an array as a (dim1, dim2) matrix of integers on a binary file pointed by 'fp' */

void write_matrix(int* A, int dim1, int dim2, FILE* fp) {
    for (int i=0; i < dim1; ++i) {
        for (int j=0; j < dim2; ++j) {
	    fprintf(fp, "%d\t", A[INDEX(i, j)]);
	}
	fprintf(fp, "\n");
    }
}

/* Main function */

int main(int argc, char* argv[]) {

    int rank = 0;
    int npes = 1;

    // intialization
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &npes);

    // local variables
    int local_N = N / npes;
    int rest = N % npes;
    if (rank < rest) {
        ++local_N;  // distribute the rest if N % npes != 0
    }

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
        // we will thereon overwrite the portion of 0, since for this program it will not be required anymore
        if (N < 10) {  // if N < 10 print on stdout
            // print the portion of 0
            print_matrix(Mat, local_N, N);
            // for all the other processes: blocking receive, compute (print)
            for (int curr_rank=1; curr_rank < npes; ++curr_rank) {
                MPI_Recv(Mat, local_N * N, MPI_INT, curr_rank, 101, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                print_matrix(Mat, local_N, N);
            }
        }
	else {  // else write on binary file
            FILE* fp = fopen("distributed_matrix1.dat", "w"); // open a file in write mode
            int local_size_buf = local_N * N * sizeof(int);  // size of the portions to write
            // write the portion of 0
            write_matrix(Mat, local_N, N, fp);
            // do the same as the above for loop, but writing on file
            for (int curr_rank=1; curr_rank < npes; ++curr_rank) {
                MPI_Recv(Mat, local_N * N, MPI_INT, curr_rank, 101, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                // write
		write_matrix(Mat, local_N, N, fp);
            }
	    fclose(fp); // release resources
        }
    }

    // deallocate and finalize
    free(Mat);

    MPI_Finalize();

    return 0;
}
