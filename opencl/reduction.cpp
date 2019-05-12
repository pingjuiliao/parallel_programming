#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<omp.h>
#include<iostream>
using namespace std;

#ifndef ARR_SIZE
#define ARR_SIZE 1000
#endif

#ifndef NUMTRIES
#define NUMTRIES 100
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
void initialize(void) ;

int
main(void) {
    int i, t;
    float a[ARR_SIZE] ;
    float b[ARR_SIZE] ;
    float c[ARR_SIZE] ;
    #ifndef _OPENMP
        fprintf(stderr, "Need OpenMp For Timing\n");
        return 1;
    #endif
    initialize() ;

    for (int n = 0 ; n < ARR_SIZE ; ++n ) {
      a[n] = Ranf(XCMIN, XCMAX) ;
      b[n] = Ranf(YCMIN, YCMAX) ;
    }

    float maxPerformance = 0. ;
    for ( t = 0 ; t < NUMTRIES ; ++t ) {

        double time0 = omp_get_wtime() ;
        float reduce = 0. ;

        for ( i = 0 ; i < ARR_SIZE ; ++i ) {
            reduce *= a[i] ;
        }
        double time1 = omp_get_wtime() ;
        double megaTrialsPerSecond = (double) ARR_SIZE / ( time1 - time0 ) / 1000000.;
        if ( megaTrialsPerSecond > maxPerformance ) {
            maxPerformance = megaTrialsPerSecond ;
        }
        // cout << "megaTrialsPerSecond : " << megaTrialsPerSecond << endl;
    }

    cout << "Max Performance : " << maxPerformance << endl ;
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
initialize(void) {
    int i ;
    struct tm y2k = { 0 } ;
    y2k.tm_hour = 0 ; y2k.tm_min = 0; y2k.tm_sec = 0;
    y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;
    time_t timer ;
    time( &timer ) ;
    double seconds = difftime( timer, mktime(&y2k) );
    unsigned int seed = (unsigned int) (1000.*seconds) ;

    srand(seed);
    return ;
}
