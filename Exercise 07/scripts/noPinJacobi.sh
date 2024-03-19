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

# This line creates / overrides a result csv file
echo "ArraySize,MegaUpdatesPerSecond,ActualRuntime,MinimalRuntime,EdgeSize,Threads" > noPinResult.csv

for thread in `seq 1 20`
do
    cd ..
    make clean
    make THREADS=$thread
    cd scripts

    srun ../bin/jacobi 4194304 1000 >> noPinResult.csv
done

#Note: copy the result.csv to a local machine!
touch terminated

