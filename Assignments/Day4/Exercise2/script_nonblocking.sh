#!/bin/bash

module load openmpi

PROCS=4

mpicc -std=c99 -o distributed_nonblocking.x distributed_nonblocking.c

mpirun -np ${PROCS} ./distributed_nonblocking.x
