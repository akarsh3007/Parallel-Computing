#!/bin/sh

RESULTDIR=result/
h=`hostname`

if [ "$h" = "mba-i1.uncc.edu"  ];
then
    echo Do not run this on the headnode of the cluster, use qsub!
    exit 1
fi

if [ ! -d ${RESULTDIR} ];
then
    mkdir ${RESULTDIR}
fi


N="1000000000"
THREADS="1 16"

make mergesort

for n in $N;
do
    for t in $THREADS;
    do
	./mergesort $n $t >/dev/null 2> ${RESULTDIR}/mergesort_${n}_${t}
    done
done

for n in $N;
do
	#output in format "thread seq par"
	     $(cat ${RESULTDIR}/mergesort_${n}_1) \
	     $(cat ${RESULTDIR}/mergesort_${n}_16)
    done   > ${RESULTDIR}/speedup_mergesort_${n}
