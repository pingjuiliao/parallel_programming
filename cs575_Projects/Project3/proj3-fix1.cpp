#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

struct s
{
  float value;
  int pad[NUM];
} Array[4];



int main(int argc, char const *argv[])
{
  omp_set_num_threads( NUMT );

  int someBigNumber = 1000000000;

  double time0 = omp_get_wtime( );
  #pragma omp parallel for
  for( int i = 0; i < 4; i++ )
  {
    for( int j = 0; j < someBigNumber; j++ )
    {
      Array[ i ].value = Array[ i ].value + 2.;
    }
  }
  double time1 = omp_get_wtime( );
  double Performance = 4.*(double)someBigNumber/(time1 - time0)/1000000.;
  printf("Performance = %8.2lf Units/Sec\n", Performance);

  return 0;
}