set title 'Jacobi on RTX 3080'
set grid
set xlabel 'Minimal Runtime [ms]'
set ylabel 'Mega Updates per second [MUP/s]'
set logscale x
set datafile separator ','
set term png size 1024,768
set output 'results.png'
set boxwidth 0.5
set style fill solid
plot 'results.csv' skip 1 using 1:3 with boxes title "Only Kernel Runtime", \
'results.csv' skip 1 using 1:4 with boxes title "Including Copy Time"