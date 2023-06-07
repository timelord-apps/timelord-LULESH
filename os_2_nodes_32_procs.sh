#!/bin/bash
#SBATCH -J lulesh_2_nodes_32_procs
#SBATCH -o lulesh.o%j
#SBATCH -e lulesh.e%j
#SBATCH -A TG-PEB220004
#SBATCH -p skx-normal
#SBATCH -N 2
#SBATCH --ntasks-per-node=32
#SBATCH -t 01:00:00

# module load gcc/7.1.0
# module load mkl/18.0.2
data="./timelord_results"
max_iter=1
export OMP_NUM_THREADS=32
export I_MPI_PIN_PROCESSOR_LIST=allcores
for ((iter=1; iter<=$max_iter; iter=iter+1))
do
    ibrun ./lulesh -s 30 -p -i 500 > $data/lulesh_os_"${iter}"_${SLURM_NTASKS}_small.txt
done


