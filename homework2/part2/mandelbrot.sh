#!/bin/bash
#$ -N Mandelbrot
#$ -q eecs221
#$ -pe mpi 32
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
for trial in 1000 2000 5000 10000 15000 20000; do
  mpirun -np 1  ./mandelbrot_serial ${trial} ${trial}
  mpirun -np 32  ./mandelbrot_joe ${trial} ${trial}
  mpirun -np 32  ./mandelbrot_susie ${trial} ${trial}
  mpirun -np 32  ./mandelbrot_ms ${trial} ${trial}
done
