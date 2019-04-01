#!/bin/bash

for nthreads in 1 2 4 8 16 20; do
    export OMP_NUM_THREADS=${nthreads}
    make
    echo ${nthreads}
    ./exercise1.x
done
