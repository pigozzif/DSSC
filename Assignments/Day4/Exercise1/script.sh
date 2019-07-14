#!/bin/bash

module load openmpi

PROCS=4

mpicc -std=c99 -o ring.x ring.c -DVECTOR

mpirun -np ${PROCS} ./ring.x 
