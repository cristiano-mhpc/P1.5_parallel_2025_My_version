#!/bin/bash 

mpic++ main.cpp -O3 -o mult.x -DDGEMM -lblas -march=native 
