This repository contains OpenMP sample use cases and examples.

# Introduction

Properly use of the omp pragma keywords (such as task, taskwait, section, 
single, etc.) can be confusing, so here I post some general use cases and 
examples.

# Source File Descriptions
Examples will self-contained in each single file. They can be compiled via: 
'g++ -fopenmp <source_file>'

nonmaster_spawn.cc: The motivation for this example was usage with 
MPI_THREAD_FUNNELED, which allows the master thread to run a normally blocking 
MPI command in the background and other non-master threads to run other parts 
of the code. See source for more details.