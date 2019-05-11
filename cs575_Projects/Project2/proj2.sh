#!/bin/bash
# number of threads:
for t in 1 2 4 8 16
do
	echo NUMTHREADS = $t

	echo == Coarse-grained parallelism and static scheduling == 
	g++ -DNUMTHREADS=$t proj2cs.cpp -o prog2 -lm -fopenmp
	./prog2

	echo == Coarse-grained parallelism and dynamic scheduling == 
	g++ -DNUMTHREADS=$t proj2cd.cpp -o prog2 -lm -fopenmp
	./prog2

	echo == Fine-grained parallelism and static scheduling == 
	g++ -DNUMTHREADS=$t proj2fs.cpp -o prog2 -lm -fopenmp
	./prog2

	echo == Fine-grained parallelism and dynamic scheduling == 
	g++ -DNUMTHREADS=$t proj2fd.cpp -o prog2 -lm -fopenmp
	./prog2


	echo 

done