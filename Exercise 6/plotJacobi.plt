set title 'jacobi'
set grid
set xlabel 'Array Size [KiB]'
set ylabel 'MegaUpdates per Second'
set logscale x
set datafile separator ','
set term png size 1024,768
set output 'jacobiResults/jacobiPlot.png'
plot 'jacobiResults/Tile.csv' skip 1 with lines title "Tile", \
'jacobiResults/Blocking.csv' skip 1 with lines title "Blocking"
