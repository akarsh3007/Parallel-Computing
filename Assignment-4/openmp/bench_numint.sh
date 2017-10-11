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


N="100 1000000000"
THREADS="1 2 4 8 16"
GRANS="1 1000"
INTENSITIES="10 1000"

make numint numint_seq

for n in $N;
do
    for gran in ${GRANS};
    do
        for intensity in $INTENSITIES;
        do
            ./numint_seq 1 0 10 ${n} ${intensity} >/dev/null 2> ${RESULTDIR}/numint_${n}_${intensity}
            for t in $THREADS;
            do
            ./numint 1 0 10 ${n} ${intensity} ${t} dynamic ${gran}  >/dev/null 2> ${RESULTDIR}/numint_${n}_${intensity}_${t}_${gran}
            done
        done
    done
done


for intensity in $INTENSITIES;
do
    for gran in ${GRANS};
    do
        for n in $N;
        do
            for thread in ${THREADS};
            do
                #output in format "gran seqtime partime"
                echo ${t} \
                $(cat ${RESULTDIR}/numint_${n}_${intensity}) \
                $(cat ${RESULTDIR}/numint_${n}_${intensity}_${t}_${gran})
            done > ${RESULTDIR}/speedup_numint_${n}_${intensity}_${gran}
            done
        done
    done
done

for intensity in $INTENSITIES;
do
    for n in $NSPLOT;
    do
	    GCMDSPN="${GCMDSPN} ; set key top left; \
                                  set xlabel 'threads'; \
                                  set ylabel 'speedup'; \
                                  set xrange [*:*]; \
                                  set yrange [*:20]; \
                                  set title 'intensity=${intensity} granularity=${gran}'; \
                plot '${RESULTDIR}/speedup_numint_${n}_${intensity}_1' u 1:(\$2/\$3) t '1' lc 4, \
                     '${RESULTDIR}/speedup_numint_${n}_${intensity}_1000' u 1:(\$2/\$3) lc 3 t '1000'; "
	done
    done
done

gnuplot <<EOF

set terminal pdf
set output 'numint_plots.pdf'

set style data linespoints

${GCMDSPN}

EOF
