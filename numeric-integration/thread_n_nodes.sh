#!/bin/bash
# number of threads:
for t in 1 2 4 8 ; do
    echo "######################"
    echo NUMT = $t
    echo -e "#####################\n"
    # number of subdivisions:
    for nds in 10 20 50 100 200 500 1000 2000 ; do
        echo NUMNODES = $nds
        g++ -DNUMNODES=$nds -DNUMT=$t prog.cpp -o prog -lm -fopenmp
        ./prog
        echo
    done
    echo
done
