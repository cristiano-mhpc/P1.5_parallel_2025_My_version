#!/bin/bash 

rm -r ./data_2/*.dat ./data/*.dat ./data_1/*.dat ./data_3/*.dat 

#valgrind --leak-check=full ./ex_rel.x
./ex_rel.x  

 
