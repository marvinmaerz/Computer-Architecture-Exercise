set title 'jacobi'
set grid
set xlabel '#Threads'
set ylabel 'MegaUpdates per Second'
set datafile separator ','
set term png size 1024,768
set output 'jacobiResults/jacobiPlot.png'
plot 'jacobiResults/multi.csv' using 6:2 skip 1 with lines title "Thread-Pinning", \
'jacobiResults/noPin.csv' using 6:2 skip 1 with lines title "No Thread-Pinning"
