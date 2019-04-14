all: prog
prog: prog.cpp
	g++ -o prog prog.cpp -lm -fopenmp
one_thread: prog.cpp
	g++ -o one_thread prog.cpp -DNUMT=1 -lm -fopenmp
four_thread: prog.cpp
	g++ -o four_thread prog.cpp -DNUMT=4 -lm -fopenmp
clean:
	rm prog
