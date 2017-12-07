#!/bin/sh

mpicxx -I mrmpi/src -c -o wc_sort.o wordcount_sort.cpp
mpicxx -o wc_sort wc_sort.o mrmpi/src/libmrmpi_mpicc.a
mpirun -n 2 ./wc_sort 1.txt
