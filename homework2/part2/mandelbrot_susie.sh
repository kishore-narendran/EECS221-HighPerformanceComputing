#!/bin/bash
#$ -N Mandelbrot
#$ -q eecs221
#$ -pe one-node-mpi 10
#$ -R y

# Grid Engine Notes:
# -----------------
# 1) Use "-R y" to request job reservation otherwise single 1-core jobs
#    may prevent this multicore MPI job from running.   This is called
#    job starvation.

# Module load boost
module load boost/1.57.0

# Module load OpenMPI
module load openmpi-1.8.3/gcc-4.9.2

# Run the program
mpirun -np 10  ./mandelbrot_susie 1000 1000
