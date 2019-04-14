#!/bin/bash
# number of threads:
for t in 1 2 4 8 ; do
    echo "######################"
    echo NUMT = $t
    echo -e "#####################\n"
    # number of subdivisions:
    for tr in 100000 1000000 5000000 10000000 ; do
        echo NUMTRIALS = $tr
        g++ -DNUMTRIALS=$tr -DNUMT=$t prog.cpp -o prog -lm -fopenmp
        ./prog
        echo
    done
    echo
done
