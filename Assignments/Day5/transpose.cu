#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

#define TYPE size_t  // macro for the type of the matrices
#define N 8192  // rows of the matrix
#define M 8192  // columns of the matrix
#define TILE 32  // size of the edge of a sub-block to transpose
#define INDEX(x, y, n) x*n + y  // since matrices are laid down as arrays


// computes wallclock time
double seconds() {
    struct timeval tmp;
    double sec;
    gettimeofday(&tmp, (struct timezone *)0);
    sec = tmp.tv_sec + ((double)tmp.tv_usec) / 1000000.0;
    return sec;
}

// transpose the input matrix into the output using the naive CUDA implementation, not shared memory
__global__ void naive_transpose(TYPE* in, TYPE* out) {
    int x = threadIdx.x;
    int y = blockIdx.x;
    // we map one block to each row, then we are sure this makes sense since the maximum number of
    // blocks is 65000^2, a very huge number. On the other side, the maximum number of threads per block
    // is 1024, not so big. Then, if the matrix is bigger than that, each thread will be mapped to more than 
    // one element, and this while loop makes sure the threads take care of all the elements they pertain to
    while (x < N) {
        out[INDEX(x, y, N)] = in[INDEX(y, x, N)];
        x += blockDim.x;
    }
}

// transpose the input matrix into the output using the block algorithm implemented in CUDA. Uses shared memory
// for better memory coalescing
__global__ void fast_transpose(TYPE* in, TYPE* out) {
    // 1. Allocate auxiliary buffer of size TILE x TILE
    __shared__ TYPE buffer[TILE][TILE];
    // recover unique indices
    int x = blockIdx.x * TILE + threadIdx.x;
    int y = blockIdx.y * TILE + threadIdx.y;
    // 2. Read data from input into shared memory tile
    for (int j=0; j < TILE; j+=blockDim.y) {
        buffer[threadIdx.y + j][threadIdx.x] = in[(y + j) * M + x];
    }
    // 3. Synchronize threads to make sure the whole tile is ready
    __syncthreads();
    // swap indices
    x = blockIdx.y * TILE + threadIdx.x;
    y = blockIdx.x * TILE + threadIdx.y;
    // 4. Transpose the local tile into output
    for (int j=0; j < TILE; j+=blockDim.y) {
        out[(y + j) * M + x] = buffer[threadIdx.x][threadIdx.y + j];
    }
}

int main(int argc, char* argv[]) {
    //define matrices
    TYPE* mat_cpu,* trans_cpu;
    TYPE* mat_gpu,* trans_gpu;
    double start, stop;
    // size of each matrix
    int size = N * M * sizeof(TYPE);

    // allocate on host
    mat_cpu = (TYPE*)malloc(size);
    trans_cpu = (TYPE*)malloc(size);
    // allocate on device
    cudaMalloc((void**)&mat_gpu, size);
    cudaMalloc((void**)&trans_gpu, size);

    // initialize matrix on host such that mat[i][j] == i + j
    for (int i=0; i < N; ++i) {
        for (int j=0; j < M; ++j) {
            mat_cpu[INDEX(i, j, N)] = (TYPE)INDEX(i, j, N);
        }
    }
    // copy result from host to device
    cudaMemcpy(mat_gpu, mat_cpu, size, cudaMemcpyHostToDevice);

    // launch naive transpose kernel
    int NTHREADS = atoi(argv[1]);  // the number of threads per block, input from command line
    start = seconds();
    naive_transpose<<<N, NTHREADS>>>(mat_gpu, trans_gpu);
    // since the kernel is synchronous with respect to the CPU, we need to call the
    // following function to make sure timings are sensible
    cudaDeviceSynchronize();
    stop = seconds();
    double s = stop - start;  // elapsed time
    // print results
    printf("Naive transpose:\n");
    printf("Elapsed time: %f seconds\nBandwidth: %f GB/s\n", s, (double(2 * size) / 1000000000.0) / s);

    // launch fast transpose kernel
    dim3 BLOCK(TILE, NTHREADS / TILE);
    dim3 GRID(N / TILE, M / TILE);
    start = seconds();
    fast_transpose<<<GRID, BLOCK>>>(mat_gpu, trans_gpu);
    cudaDeviceSynchronize();
    stop = seconds();
    s = stop - start;
    // print results
    printf("Fast transpose with shared memory:\n");
    printf("Elapsed time: %f seconds\nBandwidth: %f GB/s\n", s, (double(2 * size) / 1000000000.0) / s);

    // copy result from host to device
    cudaMemcpy(trans_cpu, trans_gpu, size, cudaMemcpyDeviceToHost);
    // print sample output
    /*for (int i=0; i < 10; ++i) {
        for (int j=0; j < 10; ++j) {
            printf("%zu ", trans_cpu[INDEX(i, j, M)]);
        }
        printf("\n");
    }*/

    // deallocate
    free(mat_cpu); free(trans_cpu);
    cudaFree(mat_gpu); cudaFree(trans_gpu);
    
    return 0;
}
