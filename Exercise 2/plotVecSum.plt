set title 'vec-sum'
set grid
set xlabel 'Array Size [KiB]'
set ylabel 'Additions per Second'
set logscale x
set datafile separator ','
set term png size 1024,768
set output 'vecSumResults/vecSumPlot.png'
plot 'vecSumResults/1Unroll.csv' skip 1 with lines title "No Unroll",\
'vecSumResults/2Unroll.csv' skip 1 with lines title "2 Unroll",\
'vecSumResults/3Unroll.csv' skip 1 with lines title "3 Unroll",\
'vecSumResults/4Unroll.csv' skip 1 with lines title "4 Unroll",\
'vecSumResults/8Unroll.csv' skip 1 with lines title "8 Unroll"
