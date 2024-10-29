reset 
set key
set grid

GRAPHFILE="times.png"

set terminal pngcairo size 640, 480 enhanced font 'Verdana, 10'
set output GRAPHFILE

#show pointsize

set xlabel "Number of threads"
set ylabel "Run times(us)"
set autoscale  

plot "timings.txt" every ::1::29 using 1:4 title 'reduction' with lines



