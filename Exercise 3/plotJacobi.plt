set title 'jacobi'
set grid
set xlabel 'Array Size [KiB]'
set ylabel 'MegaUpdates per Second'
set logscale x
set datafile separator ','
set term png size 1024,768
set output 'jacobiResults/jacobiPlot.png'
plot 'jacobiResults/1Unroll.csv' skip 1 with lines title "No Unroll",\
'jacobiResults/2Unroll.csv' skip 1 with lines title "2 Unroll + SSE Vectorization",\
'jacobiResults/4Unroll.csv' skip 1 with lines title "4 Unroll +AVX Vectorization"
