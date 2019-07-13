#!/bin/bash

cd parallel_programming

module load cudatoolkit

nvcc -o cuda.x transpose.cu

for nthreads in 64 256 512 1024; do
    ./cuda.x ${nthreads}
done
