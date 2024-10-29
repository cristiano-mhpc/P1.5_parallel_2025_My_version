#!/bin/bash

> two_threads.txt #clear the file

#threads=(1 2 3 4 5 8 16 32)

for thread in `seq 1 1 24` 
do
  export OMP_NUM_THREADS=${thread}
  ./ex_rel.x 5000

done
