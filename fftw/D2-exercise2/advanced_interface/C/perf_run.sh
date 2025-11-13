#!/bin/bash 

mpirun -n 4 bash -lc '
  rank=${OMPI_COMM_WORLD_RANK:-0}
  perf record --delay 10 -F 999 -g --call-graph dwarf -e cycles:u,instructions:u \
    -o perf.data.$rank -- ./diffusion.x
'

mkdir -p data 

mv *.dat data 

FG=/leonardo/home/userexternal/ctica000/MS_thesis/FlameGraph


for f in perf.data.*; do
  perf script -i "$f" | $FG/stackcollapse-perf.pl > "$f.folded"
  $FG/flamegraph.pl --title "Rank ${f##*.}" "$f.folded" > "flame_rank_${f##*.}.svg"
done


# (optional) merge all ranks to one overall flame:
cat perf.data.*.folded > all.folded
$FG/flamegraph.pl --title "MPI (all ranks combined)" all.folded > flame_mpi_all.svg

