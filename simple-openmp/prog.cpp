
#include <omp.h>
#include <stdio.h>
#include <math.h>

// #define NUMT	         4
#ifndef NUMT
#define NUMT 4
#endif

#define ARRAYSIZE       7000	// you decide
#define NUMTRIES        1024	// you decide

float A[ARRAYSIZE];
float B[ARRAYSIZE];
float C[ARRAYSIZE];

    int
main( )
{
#ifndef _OPENMP
    fprintf( stderr, "OpenMP is not supported here -- sorry.\n" );
    return 1;
#endif
    double maxmflops = 0.;
    double summflops = 0.;
    omp_set_num_threads( NUMT );
    fprintf( stderr, "Using %d threads\n", NUMT );

    for( int t = 0; t < NUMTRIES; t++ )
    {

        double time0 = omp_get_wtime( );
        #pragma omp parallel for
        for( int i = 0; i < ARRAYSIZE; i++ )
        {
            C[i] = A[i] * B[i];
        }
        double time1 = omp_get_wtime( );
        double mflops = (double)ARRAYSIZE/(time1-time0)/1000000.;
        summflops += mflops;
        if( mflops > maxmflops )
            maxmflops = mflops;

    }
    printf( " Peak Performance = %8.2lf MFLOPS\n", maxmflops );
    printf( "Average Performance = %8.2lf MFLOPS\n", summflops/(double)NUMTRIES );


    // note: %lf stands for "long float", which is how printf prints a "double"
    //        %d stands for "decimal integer", not "double"

    return 0;
}
