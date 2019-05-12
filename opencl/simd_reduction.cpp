#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <omp.h>
#include <iostream>
#include "simd.p4.h"

// setting the number of trials in the monte carlo simulation:
#ifndef ARR_SIZE
#define ARR_SIZE	1000
#endif

// how many tries to discover the maximum performance:
#ifndef NUMTRIES
#define NUMTRIES	10
#endif

// ranges for the random numbers:
const float XCMIN =	-1.0;
const float XCMAX =	 1.0;
const float YCMIN =	 0.0;
const float YCMAX =	 2.0;
const float RMIN  =	 0.5;
const float RMAX  =	 2.0;

// function prototypes:
float	Ranf( float , float );
int		Ranf( int, int );
void	TimeOfDaySeed( );

// main program:
int
main( int argc, char *argv[ ] )
{
#ifndef _OPENMP
	fprintf( stderr, "No OpenMP support!\n" );
	return 1;
#endif
	TimeOfDaySeed( );		// seed the random number generator

	// better to define these here so that the rand() calls don't get into the thread timing:
	float *arr1 = new float [ARR_SIZE];
	float *arr2 = new float [ARR_SIZE];
	float *arr3 = new float [ARR_SIZE];

    // fill the random-value arrays:
    for( int n = 0; n < ARR_SIZE; n++ )
    {       
            arr1[n] = Ranf( XCMIN, XCMAX );
            arr2[n] = Ranf( YCMIN, YCMAX );
    }       

    // get ready to record the maximum performance and the probability:
    float maxPerformance = 0.;      // must be declared outside the NUMTRIES loop
    // looking for the maximum performance:
    for( int t = 0; t < NUMTRIES; t++ )
    {
        double time0 = omp_get_wtime( );
        double reduction = 1.;
		SimdMulSum(arr1, arr2, ARR_SIZE) ;
        double time1 = omp_get_wtime( );
		double megaTrialsPerSecond = (double)ARR_SIZE / ( time1 - time0 ) / 1000000.;
		if( megaTrialsPerSecond > maxPerformance )
			maxPerformance = megaTrialsPerSecond;
	

        //std::cout << "megaTrialsPerSecond : " << megaTrialsPerSecond << "\n" ;
    }
    std::cout << "maxPerformance : " << maxPerformance << "\n" ; 

    return 0 ;
}

float
Ranf( float low, float high )
{
        float r = (float) rand();               // 0 - RAND_MAX
        float t = r  /  (float) RAND_MAX;       // 0. - 1.

        return   low  +  t * ( high - low );
}

int
Ranf( int ilow, int ihigh )
{
        float low = (float)ilow;
        float high = ceil( (float)ihigh );

        return (int) Ranf(low,high);
}

void
TimeOfDaySeed( )
{
	struct tm y2k = { 0 };
	y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
	y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;

	time_t  timer;
	time( &timer );
	double seconds = difftime( timer, mktime(&y2k) );
	unsigned int seed = (unsigned int)( 1000.*seconds );    // milliseconds
	srand( seed );
}
