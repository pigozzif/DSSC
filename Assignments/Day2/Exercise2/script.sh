#!/bin/bash

export OMP_NUM_THREADS=10
gcc -fopenmp -std=c99 -o exercise_2.x Exercises2.c
./exercise_2.x
