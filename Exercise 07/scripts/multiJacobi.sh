#!/bin/bash -l
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=20
#SBATCH --exclusive
#SBATCH --cpu-freq=2200000
#SBATCH -t 00:30:00
#SBATCH --nodes=1

# Enable debug and verbose mode
set -x
set -v

# Load module with icc compiler
module load intel
module load likwid

# This line creates / overrides a result csv file
echo "ArraySize,MegaUpdatesPerSecond,ActualRuntime,MinimalRuntime,EdgeSize,Threads" > multiResult.csv

for thread in `seq 1 20`
do
    cd ..
    make clean
    make THREADS=$thread
    cd scripts

    if [ $thread -le 10 ]
    then
	likwid-pin -q -c E:S0:$thread:1:1 ../bin/jacobi 4194304 1000 >> multiResult.csv
    else
	thr=$(($thread-10))
	likwid-pin -q -c E:S0:10:1:1@S1:$thr:1:1 ../bin/jacobi 4194304 1000 >> multiResult.csv
    fi
done

#Note: copy the result.csv to a local machine!
touch ready

