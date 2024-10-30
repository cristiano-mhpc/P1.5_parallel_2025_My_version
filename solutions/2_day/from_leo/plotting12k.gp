set style data histograms
set style histogram rowstacked
set style fill solid 1.0 border -1
set boxwidth 0.5
set ylabel "Total Time in (us)"
set xlabel "Number of Nodes"
set xtics rotate by -45

plot '12k.txt' using 2:xtic(1) title "communication", \
     '' using 3 title "computation"

