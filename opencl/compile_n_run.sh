#!/bin/bash
g++ -c simd.p4.cpp -o simd.p4.o

for s in 1000 10000 50000 100000 ; do
    echo "######################"
    echo NUMT = $s
    echo -e "#####################\n"
    g++ -DARR_SIZE=$s -o simd_arraymult simd_arraymult.cpp simd.p4.o -lm -fopenmp
    g++  -DARR_SIZE=$s -o arraymult arraymult.cpp -lm -fopenmp
    g++  -DARR_SIZE=$s -o simd_reduction simd_reduction.cpp simd.p4.o -lm -fopenmp
    g++  -DARR_SIZE=$s -o reduction reduction.cpp -lm -fopenmp

    ./simd_arraymult
    ./arraymult
    ./simd_reduction
    ./reduction
    echo
done
