#include <stdlib.h>
#include <stdio.h>
#include <omp.h>


/* same function as the one contained in 'loop_usage.c' */

void print_usage(int *a, int N, int nthreads) {
    for (int tid=0; tid < nthreads; ++tid) {
        fprintf(stdout, "%d:", tid);
        for (int i=0; i < N; ++i) {
            if (a[i] == tid) fprintf(stdout, "*");
            else fprintf(stdout, " ");
        }
        printf("\n");
    }
}

/* Main function. Here is where all the tests have been carried out */

int main(int argc, char* argv[]) {
    const int N = 250;
    int a[N];
    int nthreads = 10;

// test with schedule(static)
#pragma omp parallel for schedule(static)
for (int i=0; i < N; ++i) {
    a[i] = omp_get_thread_num();
}

    printf("schedule(static):\n");
    print_usage(a, N, nthreads);

// test with schedule(static, 1)
#pragma omp parallel for schedule(static, 1)
for (int i=0; i < N; ++i) {
    a[i] = omp_get_thread_num();
}

    printf("\nschedule(static, 1):\n");
    print_usage(a, N, nthreads);

// test with schedule(static, 10)
#pragma omp parallel for schedule(static, 10)
for (int i=0; i < N; ++i) {
    a[i] = omp_get_thread_num();
}

    printf("\nschedule(static, 10):\n");
    print_usage(a, N, nthreads);

// test with schedule(dynamic)
#pragma omp parallel for schedule(dynamic)
for (int i=0; i < N; ++i) {
    a[i] = omp_get_thread_num();
}

    printf("\nschedule(dynamic):\n");
    print_usage(a, N, nthreads);

// test with schedule(dynamic, 1)
#pragma omp parallel for schedule(dynamic, 1)
for (int i=0; i < N; ++i) {
   a[i] = omp_get_thread_num();
}

    printf("\nschedule(dynamic, 1):\n");
    print_usage(a, N, nthreads);

// test with schedule(dynamic, 10)
#pragma omp parallel for schedule(dynamic, 10)
for (int i=0; i < N; ++i) {
    a[i] = omp_get_thread_num();
}

    printf("\nschedule(dynamic, 10):\n");
    print_usage(a, N, nthreads);

    return 0;
}
