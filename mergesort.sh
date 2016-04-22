#!/bin/bash
#$ -N mergesort
#$ -q eecs221
#$ -pe openmp 8

# Module load gcc compiler version 4.8.2
module load  gcc/4.9.2

# Runs a bunch of standard command-line
# utilities, just as an example:

echo "Script began:" `date`
echo "Node:" `hostname`
echo "Current directory: ${PWD}"

echo ""
echo "=== Running 5 trials of Mergesort on 10 million elements ... ==="
for trial in 1 2 3 4 5 6 7 8 9 10; do
  echo "*** Trial ${trial} ***"
  ./mergesort-omp 10000000
done

echo ""
echo "=== Done! ==="

# eof
