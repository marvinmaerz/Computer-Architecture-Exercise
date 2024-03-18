#!/bin/bash -l
#SBATCH --ntasks-per-node=1
#SBATCH --exclusive
#SBATCH --cpu-freq=2200000
#SBATCH -t 00:02:00
#SBATCH --nodes=1

# Enable debug and verbose mode
set -x
set -v

# Load module with icc compiler
module load intel

# This line creates / overrides a result csv file
echo "ArraySize,MegaUpdatesPerSecond,ActualRuntime,MinimalRuntime,EdgeSize" > singleResult.csv

# run benchmark with size of 4Gib = 4194304 and minimal runtime of 1000ms

srun ../bin/jacobiSingle 4194304 1000 >> singleResult.csv

#Note: copy the result.csv to a local machine!

touch finished
