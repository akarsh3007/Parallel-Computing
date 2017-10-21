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


N="1000000000000"
THREADS="1 16"

make reduce

for n in $N;
do
    for t in $THREADS;
    do
	./reduce $n $t >/dev/null 2> ${RESULTDIR}/reduction_${n}_${t}
    done
done
	     
for n in $N;
do
    $(cat ${RESULTDIR}/reduction_${n}_1) \
	$(cat ${RESULTDIR}/reduction_${n}_16)
done   > ${RESULTDIR}/speedup_reduction_${n}

   