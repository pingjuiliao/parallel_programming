#!/bin/bash
# number of threads:
for t in 1 2 4 6 8
do
	#echo NUMT = $t
	# number of subdivisions:
	for s in 8 64 128 256 512 1024 2048 4096
	do
		#echo NUMS = $s
		g++ -DNUMS=$s -DNUMT=$t proj1.cpp -o prog -lm -fopenmp
		./prog
	done
done
