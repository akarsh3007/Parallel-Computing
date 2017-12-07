#!/bin/sh

mpicxx -I mrmpi/src -c -o wc_hist.o wordcount_hist.cpp
mpicxx -o wc_hist wc_hist.o mrmpi/src/libmrmpi_mpicc.a
mpirun -n 2 ./wc_hist 1.txt
