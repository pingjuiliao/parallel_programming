#!/bin/bash

echo ==========first==========
for t in 1024 16384 262144 524288 1048576 2097152 4194304
do
    for n in 8 32 64 128 256 512
    do
        g++ -DNUM_ELEMENTS=$t -DLOCAL_SIZE=$n -o first first.cpp /scratch/cuda-7.0/lib64/libOpenCL.so -lm -fopenmp
        ./first
    done
done


echo ==========first2==========
for t in 1024 16384 262144 524288 1048576 2097152 4194304
do
    for n in 8 32 64 128 256 512
    do
        g++ -DNUM_ELEMENTS=$t -DLOCAL_SIZE=$n -o first2 first2.cpp /scratch/cuda-7.0/lib64/libOpenCL.so -lm -fopenmp
        ./first2
    done
done

echo ==========reduce==========
    g++ -o reduce reduce.cpp /scratch/cuda-7.0/lib64/libOpenCL.so -lm -fopenmp
    ./reduce
