#!/bin/bash
#SBATCH --time=1:00
#SBATCH --partition=cpar
#SBATCH --constraint=k20


time nvprof ./bin/k_means 10000000 4 32
