all: simd.p4.o arraymult non_simd
simd.p4.o:	simd.p4.h  simd.p4.cpp
		g++  -c  simd.p4.cpp -o simd.p4.o
non_simd: arraymult.cpp
		g++ -o nonsimd_arraymult arraymult.cpp -lm -fopenmp
arraymult:	arraymult.cpp simd.p4.o
		g++  -o arraymult  arraymult.cpp simd.p4.o  -lm  -fopenmp
clean:
	rm nonsimd_arraymult arraymult simd.p4.o
