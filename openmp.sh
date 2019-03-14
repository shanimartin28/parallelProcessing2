#!/bin/sh
#PBS -A 5DV050-VT11
### Job name - defaults to name of submit script
#PBS -N OpenMP
### Output files - defaults to jobname.[eo]jobnumber
#PBS -o sequential.output
#PBS -e sequential.err
### Mail on - a=abort, b=beginning, e=end - defaults to a
#PBS -m e
### Requesting 1 nodes with 8 cores
#PBS -l nodes=1:ppn=8
### Requesting time - 5 minutes
#PBS -l walltime=00:05:00 

# Change to Working Directory
cd $PBS_O_WORKDIR
module add openmpi/gcc
echo "Using OpenMP" >>out.txt
./Mandelbrot_OpenMP 0.398318265665 0.398529534335 -0.353905644335 -0.353694375665 640 480 255 2.000000 8 >>out.txt
echo -en "\n" >>out.txt
