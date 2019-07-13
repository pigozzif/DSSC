## Exercise 6
The code can be compiled by typing `make`, and an executable named `cuda_transpose.x` will be created. In addition, the script `script.sh` contains bash code that you can launch with `qsub` (provided the queue is equipped with accelerators) and it will run all the tests of this assignment in sequence.

In this exercise we were asked to implement a matrix transposition using the CUDA API on top of the C programming language. In particular, two different kernels have been written and tested: one `naive_transpose`, which performs a matrix transposition using the textbook algorithm; and one, `fast_transpose`, which implements the block algorithm. In particular, the latter algorithm is broken down in the following steps: 1) Consider a sub-block of the original matrix; 2) copy it on an auxiliary buffer, and perform the transposition "locally"; 3) Copy the buffer in the correct position of the output matrix. Notice this exercise is also valid on the CPU, since the main issue here is being cache-friendly.

The implementation of this function has been carried out as follows. A buffer of size 32 * 32 is allocated on the `__shared__` memory for each block, which is a piece of memory accessible by all the threads of the block (but not the others) allowing very fast inter-thread communication within a given block. Subsequently, the block is transposed locally, and a call to `__syncthreads()` creates a barrier, necessary because we want to make sure that each thread has completed its job (notice that within-block synchronization comes with almost no cost). Finally, the buffer is written to the output matrix in row-wise order.

Each block transposes one of the 32x32 sub-blocks and is laid down as a 32x(THREADS_PER_BLOCK / 32) geometry. Blocks are arranged in a grid of dimensions (8192 / 32)x(8192 / 32). 

The tests have been carried out as follows. A matrix of size 8192 * 8192 was allocated, as required. Then, timings are recorded for the naive transposition and the fast one, with a varying number of threads per block (numbers investigated are 64, 256, 512, 1024). In addition, memory bandwidth can be computed GB/s. The results are as follows:

| NAIVE TRANSPOSE TIME | FAST TRANSPOSE TIME| # THREADS PER BLOCK |
|:--------------------:|:------------------:|:-------------------:|
|0.070328              |0.020498            |64                   |
|0.083234              |0.010230            |256                  |
|0.080126              |0.010540            |512                  |
|0.076519              |0.013107            |1024                 |

As we can see, the fast transpose implementation is more efficient beyond any doubt. Moreover, we are also able to compute the bandwidth of the device. Using our best timing (fast transpose with 256 threads per block), we have 104.961886 GB/s, which is considerably below the 208.0 GB/s declared for the Tesla K20m GPUs that Ulysses mounts. Notice bandwidth has been computed as ((8192 * 8192 * sizeof(size_t) * 2) * 1e+9) / seconds_elapsed, because we need to consider two matrices involved.

