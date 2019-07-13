## Exercise 6
The code can be compiled by typing `make`, and an executable named `cuda_transpose.x` will be created. In addition, the script `script.sh` contains bash code that you can launch with `qsub` (provided it is equipped with accelerators) and it will run all the tests of this assignment in sequence.

In this exercise we were asked to implement a matrix transposition using the CUDA API on top of the C programming language. In particular, two different kernels have been written and tested: one `naive_transpose`, which performs a matrix transposition using the textbook algorithm (i.e. A[i][j] = B[j][i] for every i, for every j); and one, `fast_transpose`, which implements the block algorithm. In particular, the algorithm is broken down in the following steps: 1) Consider a sub-block of the original matrix; 2) copy it on an auxiliary buffer, and perform the transposition "locally"; 3) Copy the buffer in the correct position of the output matrix. Notice this exercise is also valid on the CPU, since the main issue here is being cache-friendly.

The implementation of this function has been carried out as follows. A buffer of size 32 * 32 is allocate on the `__shared__` memory for each block, which is a piece of memory accessible by all the threads of the block (but not the others) which allows very fast inter-thread communication within a given block. Subsequently, the block is transposed locally, and a call to `__syncthreads()` creates a barrier, necessary because we want to make sure that each thread has completed its job. Finally, the buffer is written to the output matrix in row-wise order.

The tests have been carried out as follows. A matrix of size 8192 * 8192 was allocated, as required. Then, timings are recorded for the naive transposition and the fast one, with a varying number of threads per block (numbers investigated are 64, 256, 512, 1024). In addition, memory bandwidth is reported in GB/s. The results are as follows:

| NAIVE TRANSPOSE TIME | FAST TRANSPOSE TIME| # THREADS PER BLOCK |
|:--------------------:|:------------------:|:-------------------:|
|0.070328              |0.020498            |64                   |
|0.083234              |0.010230            |256                  |
|0.080126              |0.010540            |512                  |
|0.076519              |0.013107            |1024                 |



