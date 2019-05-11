#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<omp.h>
#include<iostream>
using namespace std; 

#ifndef ARR_SIZE
#define ARR_SIZE 50
#endif

#ifndef NUMTRIES
#define NUMTRIES 100
#endif





void assign_array(int *a) ;
void initialize(void) ;

int
main(void) {
    int i, t;
    int a[ARR_SIZE] ;
    int b[ARR_SIZE] ;
    int c[ARR_SIZE] ;
    #ifndef _OPENMP
        fprintf(stderr, "Need OpenMp For Timing\n");
        return 1;
    #endif
    initialize() ;
    
    assign_array(a);
    assign_array(b);
    
    float maxPerformance = 0. ;
    
    for ( t = 0 ; t < NUMTRIES ; ++t ) {
        
        double time0 = omp_get_wtime() ;
        for ( i = 0 ; i < ARR_SIZE ; ++i ) {
            c[i] = a[i] * b[i];
        }   
        double time1 = omp_get_wtime() ;
        double megaTrialsPerSecond = (double) ARR_SIZE / ( time1 - time0 ) / 1000000.;
        if ( megaTrialsPerSecond > maxPerformance ) {
            maxPerformance = megaTrialsPerSecond ;
        }
        cout << "megaTrialsPerSecond : " << megaTrialsPerSecond << endl;  
    }
    
    cout << "Max Performance : " << maxPerformance << endl ;
    return 0 ;

}

void
assign_array(int *a) {
    for (int i = 0 ; i < ARR_SIZE ; ++i) { 
        a[i] = i ;
    }
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
