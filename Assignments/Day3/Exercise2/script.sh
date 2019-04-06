#!/bin/bash

module load openmpi

mpicc -o mpi_pi.x -std=c99 mpi_pi.c

for nprocs in 1 2 4 8 16 20 40; do
    echo ${nprocs}
    mpirun -np ${nprocs} ./mpi_pi.x
done
