#!/bin/bash

module load openmpi

PROCS=4

mpicc -std=c99 -o ring.x ring.c

mpirun -np ${PROCS} ./ring.x 
