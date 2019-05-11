#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>


int	NowYear;		// 2019 - 2024
int	NowMonth;		// 0 - 11

float	NowPrecip;		// inches of rain per month
float	NowTemp;		// temperature this month
float	NowHeight;		// grain height in inches
int	NowNumDeer;		// number of deer in the current population
int NowHuman ;          // number of people

const float ONE_HUMAN_EATS_GRAIN_PER_MONTH = 0.2 ;
const int ONE_HUMAN_EATS_DEER_PER_MONTH = 1;

const float GRAIN_GROWS_PER_MONTH =		8.0;
const float ONE_DEER_EATS_PER_MONTH =		0.5;

const float AVG_PRECIP_PER_MONTH =		6.0;	// average
const float AMP_PRECIP_PER_MONTH =		6.0;	// plus or minus
const float RANDOM_PRECIP =			2.0;	// plus or minus noise

const float AVG_TEMP =				50.0;	// average
const float AMP_TEMP =				20.0;	// plus or minus
const float RANDOM_TEMP =			10.0;	// plus or minus noise

const float MIDTEMP =				40.0;
const float MIDPRECIP =				10.0;
unsigned int seed = 0xdeadbeef ;

float Ranf( unsigned int *seedp,  float low, float high ) ;
int Ranf( unsigned int *seedp, int ilow, int ihigh ) ;
void GrainDeer(void) ;
void Grain(void) ;
void Watcher(void) ;
void Human(void) ;


int
main(int argc, char** argv) {

  // starting date and time:
  NowMonth =    0;
  NowYear  = 2019;

  // starting state (feel free to change this if you want):
  NowNumDeer = 3;
  NowHeight =  1.;
  NowHuman = 2 ;

  float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );
  printf("Month\tTemperature\tPrecip\tGrain(inches)\tDeer\tHuman\n");

  float temp = AVG_TEMP - AMP_TEMP * cos( ang );
  unsigned int seed = 0;


  NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );

  float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
  NowPrecip = precip + Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );
  if( NowPrecip < 0. )
    NowPrecip = 0.;
  omp_set_num_threads( 4 );	// same as # of sections
  #pragma omp parallel sections
  {
    #pragma omp section
    {
        GrainDeer( );
    }

    #pragma omp section
    {
        Grain( );
    }

    #pragma omp section
    {
        Watcher( );
    }

    #pragma omp section
    {
        Human( );	// your own
    }
  }       // implied barrier -- all functions must return in order
  // to allow any of them to get past here
  // main() return 0 
  return 0 ;
}



void
GrainDeer(void) {

while( NowYear < 2025 )
{
	// compute a temporary next-value for this quantity
	// based on the current state of the simulation:
	int saved_num_deer = NowNumDeer ;
  if ( saved_num_deer < NowHeight ) {
    saved_num_deer ++ ;
  } else {
    saved_num_deer -- ;
  }

  if ( saved_num_deer < 0) {
    saved_num_deer = 0;
  }
	// DoneComputing barrier:
	#pragma omp barrier
	NowNumDeer = saved_num_deer ;

	// DoneAssigning barrier:
	#pragma omp barrier

	// DonePrinting barrier:
	#pragma omp barrier

}

}
void
Grain(void) {

  while( NowYear < 2025 )
  {
        // compute a temporary next-value for this quantity
  	    // based on the current state of the simulation:
        float tempFactor = expf(-powf((NowTemp-MIDTEMP)/10.,2.));
		float precipFactor = expf(-powf((NowPrecip-MIDPRECIP)/10.,2.));
		float tempHeight = NowHeight;
		tempHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
 		tempHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;
 		if(tempHeight < 0.)
 			tempHeight = 0.;

		// DoneComputing barrier:
		#pragma omp barrier
		NowHeight = tempHeight;

		// DoneAssigning barrier:
		#pragma omp barrier

		// DonePrinting barrier:
		#pragma omp barrier
  }

}

void
Watcher(void) {

  while( NowYear < 2025 )
  {
  	// compute a temporary next-value for this quantity
  	// based on the current state of the simulation:
    float	tmpPrecip = NowPrecip;		// inches of rain per month
    float	tmpTemp = NowTemp ; 		// temperature this month
    float	tmpHeight = NowHeight ;		// grain height in inches
    int	tmpNumDeer = NowNumDeer ;		// number of deer in the current population
    int tmpHuman = NowHuman ; 


    // DoneComputing barrier:
    	
    if ( tmpTemp > AVG_TEMP * 1.5 ) {
      tmpPrecip *= 1.2 ;
    }
    if ( tmpPrecip > AVG_PRECIP_PER_MONTH ) {
      NowHeight *= 1.1 ;
    }
    if ( tmpHeight >= GRAIN_GROWS_PER_MONTH * 2.0 ) {

        tmpNumDeer ++ ;
    }
    if ( 6 <= NowMonth && NowMonth <= 9 ) {
        tmpNumDeer ++ ;
        tmpHeight *= 1.21 ;
    }


    if ( tmpHeight > 20.0 ) {
        tmpHeight = 20.0 - tmpHeight * 0.001 ;
    }
    if ( tmpNumDeer >= 20 ) {
        tmpNumDeer -= 2 ;
    }
    #pragma omp barrier

		// DoneAssigning barrier:
		#pragma omp barrier
    printf("%d/%d\t%f\t%f\t%f\t%d\t%d\n", NowYear, NowMonth+1, (5./9.)*(NowTemp-32), NowPrecip, NowHeight*2.54, NowNumDeer, NowHuman);
    
    NowPrecip = tmpPrecip ;
    NowTemp = tmpTemp ;
    NowHeight = tmpHeight ;
    NowNumDeer = tmpNumDeer ;
    NowHuman = tmpHuman ; 

    NowMonth ++;
    if (NowMonth == 12)
    {
      NowMonth = 0;
      NowYear ++;
    }

    float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );
    float temp = AVG_TEMP - AMP_TEMP * cos( ang );
    NowTemp = temp + Ranf(&seed, -RANDOM_TEMP, RANDOM_TEMP);

    float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
    NowPrecip = precip + Ranf(&seed, -RANDOM_PRECIP, RANDOM_PRECIP);

    if( NowPrecip < 0. )
      NowPrecip = 0.;
		// DonePrinting barrier:
		#pragma omp barrier

  }

}
void
Human(void) {  // MyAgent

  while( NowYear < 2025 )
  {
  	// compute a temporary next-value for this quantity
  	// based on the current state of the simulation:
    float	tmpPrecip = NowPrecip;		// inches of rain per month
    float	tmpTemp = NowTemp ; 		// temperature this month
    float	tmpHeight = NowHeight ;		// grain height in inches
    int	tmpNumDeer = NowNumDeer ;		// number of deer in the current population
    int tmpHuman = NowHuman ; 
    if ( NowMonth == 10 ) {
        tmpHuman += 1; 
    }   
    if (tmpNumDeer >= 200) {
        tmpHuman += 10 ;
    }
    tmpNumDeer -= tmpHuman * ONE_HUMAN_EATS_DEER_PER_MONTH ;
    tmpHeight -= tmpHuman * ONE_HUMAN_EATS_GRAIN_PER_MONTH ;

    // +-*/
  	// DoneComputing barrier:
  	#pragma omp barrier
    NowPrecip = tmpPrecip ;
    NowTemp = tmpTemp ;
    NowHeight = tmpHeight ;
    NowNumDeer = tmpNumDeer ;
    NowHuman = tmpHuman ;
    // Now = tmp_...
  	// DoneAssigning barrier:
  	#pragma omp barrier
    
  	// DonePrinting barrier:
  	#pragma omp barrier
    // no code here 
  }

}


float
Ranf( unsigned int *seedp,  float low, float high )
{
        float r = (float) rand_r( seedp );              // 0 - RAND_MAX

        return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}


int
Ranf( unsigned int *seedp, int ilow, int ihigh )
{
        float low = (float)ilow;
        float high = (float)ihigh + 0.9999f;

        return (int)(  Ranf(seedp, low,high) );
}
