#!/bin/bash
# number of threads:
for t in 1 2 4
do
	echo ========= Fix 1 =============
	echo NUMT = $t
	for n in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
	do
		echo NUM = $n
		g++ -DNUMT=$t -DNUM=$n proj3-fix1.cpp -o prog3f1 -lm -fopenmp
		./prog3f1
	done

	echo ========= Fix 2 =============
	g++ -DNUMT=$t proj3-fix2.cpp -o prog3f2 -lm -fopenmp
	./prog3f2
	echo =============================
done