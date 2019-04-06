#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

// this function returns the seconds elapsed from the start of the Unix epoch
double seconds() {
    struct timeval tmp;
    double sec;
    gettimeofday(&tmp, (struct timezone *)0);
    sec = tmp.tv_sec + ((double) tmp.tv_usec) / 1000000.0;
    return sec;
}


int main(int argc, char* argv[]) {
    double N = 1000000000;  // large number of rectangles
    double h = 1 / N;  // step
    int rank = 0;
    int npes = 1;
    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &npes);
    
    if (rank == 0) start_time = seconds();  // time has to be taken on one process, for us 0

    double local_result[1] = {0.0};  // input buffer for the reduce operation
    double global_result[1] = {0.0};  // output buffer for the reduce operation
    int start = rank * (N / npes);  // local start
    int end = start + (N / npes);  // local end
    // compute estimate of pi on local portion
    for (int i=start; i < end; ++i) {
         local_result[0] += 4.0 / (1.0 + (h * (i + 0.5)) * (h * (i + 0.5)));
    }

    local_result[0] *= h;
    
    // perform reduction on process size-1
    MPI_Reduce(local_result, global_result, 1, MPI_DOUBLE, MPI_SUM, npes-1, MPI_COMM_WORLD);
    
    // take end time, so that to cut send/recv portion
    if (rank == 0) {
        end_time = seconds();
    }
    
    // send the result from process size-1 to process 0, which will print it
    if (rank == npes - 1) {
        MPI_Send(global_result, 1, MPI_DOUBLE, 0, 101, MPI_COMM_WORLD);
    }
    if (rank == 0) {
        MPI_Recv(global_result, 1, MPI_DOUBLE, npes - 1, 101, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Result: %f\nElapsed time: %f\n", global_result[0], end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}
