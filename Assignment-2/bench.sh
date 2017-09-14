#!/bin/sh

h=`hostname`

if [ "$h" = "mba-i1.uncc.edu"  ];
then
    echo Do not run this on the headnode of the cluster, use qsub!
    exit 1
fi

INTENSITIES="1 10 100 1000"
NS="`seq 1 10` `seq 20 10 100` `seq 200 100 1000` `seq 2000 1000 10000` `seq 20000 10000 100000` `seq 200000 100000 1000000` `seq 2000000 1000000 10000000` `seq 20000000 10000000 100000000`"
#NS="`seq 1 10`"

for intensity in $INTENSITIES;
do
    for n in $NS;
    do
	echo $n\ $(./main 1 0 10 ${n} ${intensity} 2>&1 >/dev/null)
    done > time_inten_${intensity}
done 

gnuplot <<EOF
set terminal pdf
set output 'time_plots.pdf'

set xlabel 'n'
set ylabel 'time (in seconds)'

set style data linespoints

set key top left
plot 'time_inten_1' t "intensity=1" lc 1, \
     'time_inten_10' t "intensity=10" lc 6, \
     'time_inten_100' t "intensity=100" lc 3, \
     'time_inten_1000' t "intensity=1000" lc 4

plot 'time_inten_1' t "intensity=1" lc 1

plot 'time_inten_10' t "intensity=10" lc 6

plot 'time_inten_100' t "intensity=100" lc 3

plot 'time_inten_1000' t "intensity=1000" lc 4

EOF
