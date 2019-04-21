#!/bin/bash

module load openmpi

mpicc -std=c99 -o distributed_blocking.x distributed_blocking.c

mpirun -np ${PROCS} ./distributed_blocking.x
