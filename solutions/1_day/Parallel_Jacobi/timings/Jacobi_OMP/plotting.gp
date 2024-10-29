reset 
set key
set grid

GRAPHFILE="times.png"

set terminal pngcairo size 640, 480 enhanced font 'Verdana, 10'
set output GRAPHFILE

#show pointsize

set xlabel "Threads"
set ylabel "Run times(us)"
set autoscale  

plot "timing.txt" using 1:2 title 'Timing for 1000by1000' with lines, \

