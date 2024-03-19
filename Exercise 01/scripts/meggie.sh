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
echo "ArraySize,MegaUpdatesPerSecond,ActualRuntime,MinimalRuntime,EdgeSize" > result.csv

# run benchmark 1
# execute measurement with for loop
# 32 measurement points, exponentially distributed: 1 KiB - 128 MiB, each with 1000 ms as minimal runtime
# results should be appended to the result.csv (see >> operator)

#exponential function used for sizes (32 entries): f(x) = 0.67713 * 2**(0.5489*x)
sizes=(1 1 2 3 4 6 9 14 20 30 44 65 95 139 203 298 436 638 933 1365 1998 2923 4276 6256 9152 13390 19589 28659 41927 61383 89736 131072)

for size in ${sizes[@]}
do
    srun ../bin/jacobi $size 1000 >> result.csv
done

#Note: copy the result.csv to a local machine!
touch ready

