#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <sys/time.h>

/* this function returns the seconds elapsed since the start of the Unix epoch on January 1, 1970 */

double seconds() {
  struct timeval tmp;
  double sec;
  gettimeofday(&tmp, (struct timezone *)0);
  sec = tmp.tv_sec + ((double)tmp.tv_usec) / 1000000.0;
  return sec;
}

/* Main function */

int main(int argc, char* argv[]) {

    // initialization of global variables
    double global_result = 0.0;
    int N = 1000000000;
    double h = 1.0 / N;  // step

    printf("Test with critical:\n");
    
    double start = seconds();

    // start of the OpenMP parallel region; test with 'critical'
    #pragma omp parallel
    {
        double curr, local_result;
        
        // work-sharing construct to split the computation of the different
        // rectangles
        #pragma omp for
        for (int i=0; i < N; ++i) {
            curr = h * (i + 0.5);
            //#pragma omp critical    // protect against a race condition
            local_result += 1.0 / (1.0 + curr*curr);
        }
        #pragma omp critical
        global_result += local_result;
        // implicit barrier upon exit of the work-sharing construct
        //double job_duration = omp_get_wtime() - tstart;
        //#pragma omp atomic
        //avg_job_duration += job_duration / omp_get_num_threads();   // omp_get_num_threads() does not have race conditions
    }
    global_result *= 4.0 * h;
    
    double end = seconds();
    printf("Result: %f\n", global_result);
    printf("Time elapsed: %f\n", end - start);
    // re-initialize
    global_result = 0.0;
    //avg_job_duration = 0.0;
    printf("Test with atomic:\n");
    start = seconds();
    // start of the OpenMP parallel region; test with 'atomic'
    #pragma omp parallel
    {
        double curr, local_result;
//        double tstart = omp_get_wtime();

        // work-sharing construct to split the computation of the different
        // rectangles
        #pragma omp for
        for (int i=0; i < N; ++i) {
            curr = h * (i + 0.5);
            //#pragma omp atomic    // protect against a race condition
            local_result += 1.0 / (1.0 + curr*curr);
        }
        // implicit barrier upon exit of the work-sharing construct
        #pragma omp atomic
        global_result += local_result;
    }
    global_result *= 4.0 * h;
    end = seconds();
    printf("Result: %f\n", global_result);
    printf("Time elapsed: %f\n", end - start);

    // re-initialize
    global_result = 0.0;
    //avg_job_duration = 0.0;
    printf("Test with reduction:\n");
    start = seconds();
    // start of the OpenMP parallel region; test with reduction, as seen in the following line
    #pragma omp parallel reduction(+:global_result)
    {
        double curr, local_result;
        //double tstart = omp_get_wtime();

        // work-sharing construct to split the computation of the different
        // rectangles
        #pragma omp for
        for (int i=0; i < N; ++i) {
            curr = h * (i + 0.5);
            local_result += 1.0 / (1.0 + curr*curr);
        }
        global_result += local_result;
        // implicit barrier upon exit of the work-sharing construct
        //double job_duration = omp_get_wtime() - tstart;
        //#pragma omp atomic
        //avg_job_duration += job_duration / omp_get_num_threads();
    }
    
    global_result *= 4.0 * h;
    end = seconds();
    printf("Result: %f\n", global_result);
    printf("Time elapsed: %f\n", end - start);

    return 0;
}
