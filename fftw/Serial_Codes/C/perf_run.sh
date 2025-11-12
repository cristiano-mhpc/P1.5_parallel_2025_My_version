#!/bin/bash 

perf record -F 999 -g --call-graph dwarf -- ./diffusion.x 

perf script | /home/christian/Desktop/MHPC_LECTURES/MS_THESIS/profile/perf/FlameGraph/stackcollapse-perf.pl > out.folded

/home/christian/Desktop/MHPC_LECTURES/MS_THESIS/profile/perf/FlameGraph/flamegraph.pl --title "Diffusion Serial" out.folded > flame_serial.svg 
