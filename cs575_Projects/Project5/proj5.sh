#!/bin/bash
# number of threads:
for t in 1 2 4
do
	#echo NUMT = $t
	# data set size:
	for s in 1000 10000 100000 1600000 32000000 64000000
	do
		#echo NUMS = $s
		icpc -DNUMS=$s -DNUMT=$t -o proj project5.cpp -lm -openmp -align -qopt-report=3 -qopt-report-phase=vec
		./proj
	done
done
