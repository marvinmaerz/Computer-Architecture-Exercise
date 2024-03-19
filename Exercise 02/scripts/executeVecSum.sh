#!/bin/bash -l
#SBATCH --ntasks-per-node=1
#SBATCH --exclusive
#SBATCH --cpu-freq=2200000
#SBATCH -t 00:30:00
#SBATCH --nodes=1

module load intel

cd ..

unrolls=(1 2 3 4 8)

for unroll in ${unrolls[@]}
do
	make clean
	make NUM=${unroll}

	cd scripts
	sbatch meggieVecSum.sh

	while [ ! -f ready ]
	do
		sleep 5
	done
	
	find . -name 'meggieVecSum.sh.*' -exec rm {} \;
	rm ready
	mv resultVecSum.csv ../vecSumResults/${unroll}Unroll.csv

	cd ..
done

#to plot type: gnuplot plotVecSum.plt

touch done
