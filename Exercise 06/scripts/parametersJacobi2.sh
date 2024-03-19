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
echo "ArraySize,MegaUpdatesPerSecond,ActualRuntime,MinimalRuntime,EdgeSize,Bx,By" > parameterResult2.csv

# run benchmark 1
# execute measurement with for loop
# 32 measurement points, exponentially distributed: 1 MiB - 16 GiB, each with 1000 ms as minimal runtime
# results should be appended to the result.csv (see >> operator)

#exponential function used for sizes (32 entries): f(x) = 0.566*2**(0.46316*x)
width=(552 560 568 576 584 592 600 608 616 624 632 640 648 676 684 692 700)
hight=(4 8 12 16 20 24 28 32 36 40 44 48 52)
for bx in ${width[@]}
do
	for by in ${hight[@]}
	do
		make clean
		make TBX=$bx BY=$by
		srun ./bin/jacobiTile 524288 1000 >> parameterResult2.csv
	done
done

make clean
#Note: copy the result.csv to a local machine!
touch ready

