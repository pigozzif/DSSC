#!/bin/bash

module load openmpi

PROCS=4

mpicc -std=c99 -o distributed_blocking.x distributed_blocking.c

mpirun -np ${PROCS} ./distributed_blocking.x
