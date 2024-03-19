#!/bin/bash -l
#SBATCH --ntasks-per-node=1
#SBATCH --exclusive
#SBATCH --cpu-freq=2200000
#SBATCH -t 00:30:00
#SBATCH --nodes=1

module load intel
module load likwid

cd ..

make clean
make 

cd scripts
sbatch singleJacobi.sh
sbatch multiJacobi.sh

while [ ! -f ready ]
do
	sleep 5
done

sbatch noPinJacobi.sh

while [ ! -f finished ]
do 
	sleep 5
done

while [ ! -f terminated ]
do
	sleep 5
done

find . -name 'singleJacobi.sh.*' -exec rm {} \;
find . -name 'multiJacobi.sh.*' -exec rm {} \;
find . -name 'noPinJacobi.sh.*' -exec rm {} \;
rm ready
rm finished
rm terminated
mv singleResult.csv ../jacobiResults/single.csv
mv multiResult.csv ../jacobiResults/multi.csv
mv noPinResult.csv ../jacobiResults/noPin.csv
cd ..

make clean
#to plot type: gnuplot plotJacobi.plt

touch done
