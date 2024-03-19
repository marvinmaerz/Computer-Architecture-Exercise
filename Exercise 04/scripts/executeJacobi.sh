#!/bin/bash -l
#SBATCH --ntasks-per-node=1
#SBATCH --exclusive
#SBATCH --cpu-freq=2200000
#SBATCH -t 00:30:00
#SBATCH --nodes=1

module load intel

cd ..

make clean
make

cd scripts
sbatch meggieJacobi.sh

while [ ! -f ready ]
do
	sleep 5
done

find . -name 'meggieJacobi.sh.*' -exec rm {} \;
rm ready
mv resultJacobi.csv ..

cd ..

#to plot type: gnuplot plotJacobi.plt

touch done
