#!/bin/bash -l
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=20
#SBATCH --exclusive
#SBATCH --cpu-freq=2200000
#SBATCH -t 00:30:00
#SBATCH --nodes=1

# debug and verbose mode
set -x
set -v

module load intel
module load likwid

salloc

cd ..
make clean
make cpuStream
cd scripts

# with and without thread pinning
touch ../results/cpuStream-pinning.txt
likwid-pin -q -c E:S0:10:1:1@S1:10:1:1 ../bin/cpuStream >> ../results/cpuStream-pinning.txt

touch ../results/cpuStream-nopin.txt
srun ../bin/cpuStream >> ../results/cpuStream-nopin.txt

touch ready

