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

    printf("\nTest with atomic:\n");

    double start = seconds();

    // start of the OpenMP parallel region
    #pragma omp parallel
    {
        double curr, local_result;

        // work-sharing construct to split the computation of the different
        // rectangles
        #pragma omp for
        for (int i=0; i < N; ++i) {
            curr = h * (i + 0.5);
            local_result += 1.0 / (1.0 + curr*curr);
        }
        #pragma omp atomic
        global_result += local_result;  // possible race condition
    }
    global_result *= 4.0 * h;
    
    // print results
    double end = seconds();
    
    printf("Result: %f\n", global_result);
    printf("Time elapsed: %f\n", end - start);

    return 0;
}
