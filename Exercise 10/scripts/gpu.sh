#!/bin/bash -l
#SBATCH --ntasks-per-node=1
#SBATCH --exclusive
#SBATCH --cpus-per-task=1
#SBATCH --cpu-freq=2200000
#SBATCH -t 00:30:00
#SBATCH --nodes=1
#SBATCH --gres=gpu:rtx3080:1

# debug and verbose mode
set -x
set -v

module load cuda
module load intel

cd ..
make clean
make 
cd scripts

# with and without thread pinning
touch gpu.txt
srun ../bin/gpuDGEMM >> gpu.txt

touch ready

