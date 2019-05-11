#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <time.h>

float C[NUMS];

int main( int argc, char *argv[ ] ){

	#ifndef _OPENMP
        fprintf( stderr, "OpenMP is not supported here -- sorry.\n" );
        return 1;
	#endif

    omp_set_num_threads( NUMT );
	fprintf( stderr, "Using %d threads\n", NUMT );
	fprintf( stderr, "Data Set Size: %d \n", NUMS );

	float *A = new float [NUMS];

	for( int i = 0; i < NUMS; i++ ){

		A[i] = rand();
	}

	double time0 = omp_get_wtime( );

	#pragma omp parallel for
	for( int i = 0; i < NUMS; i++ ){
		C[i] = sqrt( A[i] );
	}

	double time1 = omp_get_wtime( );

	double megaMults = (double)(NUMS)/(time1-time0)/1000000.;
	printf( "Performance = %8.2lf MegaMults/Sec\n", megaMults );
	
}