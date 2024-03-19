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
echo "ArraySize,MegaUpdatesPerSecond,ActualRuntime,MinimalRuntime,EdgeSize" > blockingResult.csv

# run benchmark 1
# execute measurement with for loop
# 32 measurement points, exponentially distributed: 1 MiB - 16 GiB, each with 1000 ms as minimal runtime
# results should be appended to the result.csv (see >> operator)

#exponential function used for sizes (32 entries): f(x) = 0.566*2**(0.46316*x)
sizes=(1024 1024 2048 3072 4096 5120 6144 8192 10240 14336 19456 27648 37888 51200 71680 98304 136192 187392 258048 356352 491520 676864 932864 1286144 1772544 1420288 3368960 4645888 6404096 8827904 12170240 16777216)

for size in ${sizes[@]}
do
    srun ../bin/jacobiBlocking $size 1000 >> blockingResult.csv
done

#Note: copy the result.csv to a local machine!
touch finished

