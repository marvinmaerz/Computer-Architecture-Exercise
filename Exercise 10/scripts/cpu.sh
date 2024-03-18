#!/bin/bash -l
#SBATCH --ntasks-per-node=1
#SBATCH --exclusive
#SBATCH --cpu-freq=2200000
#SBATCH -t 00:30:00
#SBATCH --nodes=1

# Enable debug and verbose mode
set -x
set -v

module load intel

touch cpu.txt
srun ../bin/cpuDGEMM > cpu.txt

touch ready

