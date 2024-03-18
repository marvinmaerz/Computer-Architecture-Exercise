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
echo "ArraySize,AdditionsPerSecond,ActualRuntime,MinimalRuntime" > timeResult.csv

# run benchmark 1, for different minimal runtimes
# execute measurement with for loop
# 32 measurement points, exponentially distributed: 1s (1000ms) - 64s, each with 1000 KiB as fixed array size
# results should be appended to the result.csv (see >> operator)

#exponential function used for times (32 entries): f(x) = e**(0.1341575 * x)
times=(1 1 1 1 1 1 2 2 2 3 3 4 5 5 6 7 8 9 11 12 14 16 19 21 25 28 32 37 42 48 56 64)

for time in ${times[@]}
do
    time=$((time*1000))                           #conversion from time[s] to time[ms]
    srun ../bin/vecSum 1000 $time >> timeResult.csv
done

#Note: copy the result.csv to a local machine!
touch ready

