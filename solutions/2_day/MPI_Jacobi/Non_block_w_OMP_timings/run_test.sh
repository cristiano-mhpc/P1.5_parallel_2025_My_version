#!/bin/bash 

rm -r ./data/*.dat 

export OMP_NUM_THREADS=8

mpirun -np 4 ./ex_rel.x $1
