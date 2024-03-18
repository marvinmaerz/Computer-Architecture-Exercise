#!/bin/bash -l
#SBATCH --ntasks-per-node=1
#SBATCH --exclusive
#SBATCH --cpu-freq=2200000
#SBATCH -t 01:30:00
#SBATCH --nodes=1

# Enable debug and verbose mode
set -x
set -v

# Load module with icc compiler
module load intel

# allocate a compute node -> takes SBATCH arguments?
cd ..

# This line creates / overrides a result csv file
echo "ArraySize,MegaUpdatesPerSecond,ActualRuntime,MinimalRuntime,EdgeSize,Bx,By" > parameterResult.csv

# run benchmark 1
# execute measurement with for loop
# 32 measurement points, exponentially distributed: 1 MiB - 16 GiB, each with 1000 ms as minimal runtime
# results should be appended to the result.csv (see >> operator)

#exponential function used for sizes (32 entries): f(x) = 0.566*2**(0.46316*x)
width=(20 40 80 100 200 300 400 500 600 700 800 900 1000 1100 1200 1300 1400 1500)
for bx in ${width[@]}
do
	for by in ${width[@]}
	do
		make clean
		make TBX=$bx BY=$by
		srun ./bin/jacobiTile 524288 1000 >> parameterResult.csv
	done
done

make clean
#Note: copy the result.csv to a local machine!
touch ready

