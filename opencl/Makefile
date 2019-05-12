all: simd.p4.o arraymult reduction 

simd.p4.o:	simd.p4.h  simd.p4.cpp
		g++  -c  simd.p4.cpp -o simd.p4.o

arraymult:	arraymult.cpp simd.p4.o
		g++  -o simd_arraymult arraymult.cpp simd.p4.o  -lm  -fopenmp
		g++  -o arraymult arraymult.cpp -lm -fopenmp

reduction: reduction.cpp simd.p4.o
		g++ -o simd_reduction reduction.cpp simd.p4.o -lm -fopenmp
		g++ -o reduction reduction.cpp -lm -fopenmp
clean :
	rm simd.p4.o reduction arraymult
