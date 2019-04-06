#!/bin/bash

module load openmpi

mpicc -std=c99 -o bcast.x bcast.c

mpirun -np 4 ./bcast.x
