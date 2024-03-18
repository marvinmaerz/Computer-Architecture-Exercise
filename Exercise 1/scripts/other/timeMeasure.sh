#!/bin/bash -l
#SBATCH --ntasks-per-node=1
#SBATCH --exclusive
#SBATCH --cpu-freq=2200000
#SBATCH -t 00:30:00
#SBATCH --nodes=1

# Enable debug and verbose mode
set -x
set -v

# Load module with icc compiler
module load intel

# allocate a compute node -> takes SBATCH arguments?
salloc

# This line creates / overrides a result csv file
echo "ArraySize,MegaUpdatesPerSecond,ActualRuntime,MinimalRuntime,EdgeSize" > timeMeasure.csv

# run benchmark 1, for different minimal runtimes
# execute measurement with for loop
# 32 measurement points along 1ms - 10000ms (10s), each with 1000 KiB as fixed array size
# results should be appended to the result.csv (see >> operator)

times=(1 10 20 30 40 50 60 70 80 90 100 150 200 250 300 400 600 700 800 1000 1500 2000 2500 3000 4000 5000 6000 7000 8000 9000 10000 15000)

for time in ${times[@]}
do
    srun ../bin/jacobi 1000 $time >> timeMeasure.csv
done

#Note: copy the result.csv to a local machine!
touch ready

