#!/bin/sh

mpicxx -I mrmpi/src -c -o wc.o wordcount.cpp
mpicxx -o wc wc.o mrmpi/src/libmrmpi_mpicc.a
mpirun -n 2 ./wc 1.txt
