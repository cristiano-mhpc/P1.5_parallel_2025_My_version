#!/bin/bash

threads=({1..32})
sizes=(100 1000 10000000)

for j in ${sizes[@]}
do

  > timings.txt #clear the file

  for i in ${threads[@]}  
  do
    export OMP_NUM_THREADS=${i}
    echo -n $i " " 
    ./area.x ${j}

  done > timings.txt

done
