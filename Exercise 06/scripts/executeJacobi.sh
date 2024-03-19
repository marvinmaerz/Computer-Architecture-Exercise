#!/bin/bash -l
#SBATCH --ntasks-per-node=1
#SBATCH --exclusive
#SBATCH --cpu-freq=2200000
#SBATCH -t 00:30:00
#SBATCH --nodes=1

module load intel

cd ..

make clean
make BX=7724 TBX=608 BY=20

cd scripts
sbatch tileJacobi.sh
sbatch blockingJacobi.sh

while [ ! -f ready ]
do
	sleep 5
done

while [ ! -f finished ]
do 
	sleep 5
done

find . -name 'tileJacobi.sh.*' -exec rm {} \;
find . -name 'blockingJacobi.sh.*' -exec rm {} \;
rm ready
rm finished
mv tileResult.csv ../jacobiResults/Tile.csv
mv blockingResult.csv ../jacobiResults/Blocking.csv

cd ..

make clean
#to plot type: gnuplot plotJacobi.plt

touch done
