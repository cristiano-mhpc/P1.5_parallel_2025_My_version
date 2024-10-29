reset 
set key
set grid

GRAPHFILE="times.png"

set terminal pngcairo size 640, 480 enhanced font 'Verdana, 10'
set output GRAPHFILE

#show pointsize

set xlabel "Number of rows"
set ylabel "Run times(us)"
set autoscale  

plot "../2_Jacob_parallel/two_threads.txt" using 1:2 title 'Boss Battle' with lines, \
"../2_Jacob_sequential/two_threads.txt" using 1:2 title 'Not Parallel' with lines, \
"../2_Jacob_par_unseq/two_threads.txt" using 1:2 title 'par_unseq' with lines
#"../2_Jacob_par_w_index_vector/two_threads.txt" using 1:2 title 'With index vector' with lines,\

