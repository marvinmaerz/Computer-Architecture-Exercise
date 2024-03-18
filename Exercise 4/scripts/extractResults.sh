#!/bin/bash -l
#SBATCH --ntasks-per-node=1
#SBATCH --exclusive
#SBATCH --cpu-freq=2200000
#SBATCH -t 00:30:00
#SBATCH --nodes=1

sizes=(1 1 2 3 4 6 9 14 20 30 44 65 95 139 203 298 436 638 933 1365 1998 2923 4276 6256 9152 13390 19589 28659 41927 61383 89736 131072)
#for loop 
#arr=($LOADS)
LOADS=$(cat result.csv | grep MEM_UOPS_RETIRED_LOADS | grep STAT | cut -d, -f5)
HITS=$(cat result.csv | grep MEM_LOAD_UOPS_RETIRED_L1 | grep STAT | cut -d, -f5)
loads=($LOADS)
hits=($HITS)
echo "ArraySize, Hits, Loads" > endResult.csv
for i in {0..31}
do
	echo "${sizes[i]}, ${hits[i]}, ${loads[i]}" >> endResult.csv
done
mv endResult.csv ../results/rowJacobi.csv

LOADS=$(cat newResult.csv | grep MEM_UOPS_RETIRED_LOADS | grep STAT | cut -d, -f5)
HITS=$(cat newResult.csv | grep MEM_LOAD_UOPS_RETIRED_L1 | grep STAT | cut -d, -f5)
loads=($LOADS)
hits=($HITS)
echo "ArraySize, Hits, Loads" > newEndResult.csv
for i in {0..31}
do
	echo "${sizes[i]}, ${hits[i]}, ${loads[i]}" >> newEndResult.csv
done
mv newEndResult.csv ../results/columnJacobi.csv

touch executed
#to plot type: gnuplot plotJacobi.plt

