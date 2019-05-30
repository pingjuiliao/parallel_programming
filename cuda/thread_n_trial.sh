#!/bin/bash
# number of threads:
CC=/usr/local/apps/cuda/cuda-9.2/bin/nvcc
for b in 16 32 64 ; do
    echo "######################"
    echo BLOCKSIZE = $b
    echo -e "#####################\n"
    # number of subdivisions:
    for tr in 16384 32768 65536 131072 262144 524288 ; do
        echo NUMTRIALS = $tr
        $CC -DNUMTRIALS=$tr -DBLOCKSIZE=$b montecarlo.cu -o montecarlo
	./montecarlo
        echo
    done
    echo
done
