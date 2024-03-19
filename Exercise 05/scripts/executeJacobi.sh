#!/bin/bash -l
#SBATCH --ntasks-per-node=1
#SBATCH --exclusive
#SBATCH --cpu-freq=2200000
#SBATCH -t 00:30:00
#SBATCH --nodes=1

module load intel
module load likwid

cd ..

unrolls=4 #(1 2 4)

<<<<<<< HEAD:Exercise 4/scripts/executeJacobi.sh
for unroll in ${unrolls[@]}
=======
cd scripts
sbatch meggieJacobi.sh
sbatch blockingJacobi.sh

while [ ! -f ready ]
>>>>>>> main:Exercise 5/scripts/executeJacobi.sh
do
	make clean
	make NUM=${unroll}

	cd scripts
	sbatch meggieJacobi.sh

	while [ ! -f ready ]
	do
		sleep 5
	done
	
	#find . -name 'meggieJacobi.sh.*' -exec rm {} \;
	#rm ready
	#mv resultJacobi.csv ../jacobiResults/${unroll}Unroll.csv

	cd ..
done

<<<<<<< HEAD:Exercise 4/scripts/executeJacobi.sh
=======
while [ ! -f finished ]
do 
	sleep 5
done

find . -name 'meggieJacobi.sh.*' -exec rm {} \;
find . -name 'blockingJacobi.sh.*' -exec rm {} \;
rm ready
rm finished
mv resultJacobi.csv ../jacobiResults/NoBlocking.csv
mv blockingResult.csv ../jacobiResults/Blocking.csv

cd ..

make clean
>>>>>>> main:Exercise 5/scripts/executeJacobi.sh
#to plot type: gnuplot plotJacobi.plt

#touch done
