#!/bin/bash

> timings.txt #clear the file

threads=(1 2 3 4 5 6 7 8 9 10)

for i in ${threads[@]} #`seq 2 5 10` 
do
  export OMP_NUM_THREADS=${i}
  echo -n $i " " 
  ./area.x 100000000
done > timings.txt

 
