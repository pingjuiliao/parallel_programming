// Array multiplication: C = A * B:

// System includes
#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <math.h>
#include <stdlib.h>

// CUDA runtime
#include <cuda_runtime.h>

// Helper functions and utilities to work with CUDA
#include "helper_functions.h"
#include "helper_cuda.h"

#ifndef BLOCKSIZE
#define BLOCKSIZE		32		// number of threads per block
#endif

#ifndef SIZE
#define SIZE			1*8096	// array size
#endif

#ifndef NUMTRIALS
#define NUMTRIALS		100		// to make the timing more accurate
#endif

#ifndef TOLERANCE
#define TOLERANCE		0.00001f	// tolerance to relative error
#endif

const float XCMIN = 0.0 ;
const float XCMAX = 2.0 ;
const float YCMIN = 0.0 ;
const float YCMAX = 2.0 ;
const float RMIN  = 0.5 ;
const float RMAX  = 2.0 ;

// function prototypes 
float Ranf(float, float);

// array multiplication (CUDA Kernel) on the device: C = A * B

__global__  void ArrayMul( float *A, float *B, float *C )
{
	__shared__ float prods[BLOCKSIZE];

	unsigned int numItems = blockDim.x;
	unsigned int tnum = threadIdx.x;
	unsigned int wgNum = blockIdx.x;
	unsigned int gid = blockIdx.x*blockDim.x + threadIdx.x;

	prods[tnum] = A[gid] * B[gid];

	for (int offset = 1; offset < numItems; offset *= 2)
	{
		int mask = 2 * offset - 1;
		__syncthreads();
		if ((tnum & mask) == 0)
		{
			prods[tnum] += prods[tnum + offset];
		}
	}

	__syncthreads();
	if (tnum == 0)
		C[wgNum] = prods[0];
}


__global__ void MonteCarlo( float *xcs, float *ycs, float *rs, unsigned int *p_numHits )  
{	
	__shared__ unsigned int hits[BLOCKSIZE];
	unsigned int numItems = blockDim.x;
	unsigned int tnum = threadIdx.x;
	unsigned int wgNum = blockIdx.x;
	unsigned int gid = blockIdx.x*blockDim.x + threadIdx.x;
	
	// c numHits == 0

	// randomize the location and radius of the circle:
	float xc = xcs[gid];
	float yc = ycs[gid];
	float  r =  rs[gid];

	// solve for the intersection using the quadratic formula:
	float a = 2.;
	float b = -2.*( xc + yc );
	float c = xc*xc + yc*yc - r*r;
	float d = b*b - 4.*a*c;
	// If d is less than 0., then the circle was completely missed. (Case A) Continue on to the next trial in the for-loop.
	if ( d >= 0. ) {

		// hits the circle:
		// get the first intersection:
		d = sqrt( d );
		float t1 = (-b + d ) / ( 2.*a );	// time to intersect the circle
		float t2 = (-b - d ) / ( 2.*a );	// time to intersect the circle
		float tmin = t1 < t2 ? t1 : t2;		// only care about the first intersection
		// If tmin is less than 0., then the circle completely engulfs the laser pointer. (Case B) Continue on to the next trial in the for-loop.
		if ( tmin >= 0. ) {


			// where does it intersect the circle?
			float xcir = tmin;
			float ycir = tmin;

			// get the unitized normal vector at the point of intersection:
			float nx = xcir - xc;
			float ny = ycir - yc;
			float n = sqrt( nx*nx + ny*ny );
			nx /= n;	// unit vector
			ny /= n;	// unit vector

			// get the unitized incoming vector:
			float inx = xcir - 0.;
			float iny = ycir - 0.;
			float in = sqrt( inx*inx + iny*iny );
			inx /= in;	// unit vector
			iny /= in;	// unit vector

			// get the outgoing (bounced) vector:
			float dot = inx*nx + iny*ny;
			// float outx = inx - 2.*nx*dot;	// angle of reflection = angle of incidence`
			float outy = iny - 2.*ny*dot;	// angle of reflection = angle of incidence`

			// find out if it hits the infinite plate:
			float t = ( 0. - ycir ) / outy;
			// If t is less than 0., then the reflected beam went up instead of down. Continue on to the next trial in the for-loop.
			if ( t >= 0. ) {
				hits[tnum] ++;
			}
			//  Otherwise, this beam hit the infinite plate. (Case D) Increment the number of hits and continue on to the next trial in the for-loop.

		}
	}
	for (int offset = 1; offset < numItems; offset *= 2)
	{
		int mask = 2 * offset - 1;
		__syncthreads();
		if ((tnum & mask) == 0)
		{
			hits[tnum] += hits[tnum + offset];
		}
	}

	__syncthreads();
	if (tnum == 0)
		p_numHits[wgNum] = hits[0];
}

// main program:

	int
main( int argc, char* argv[ ] )
{
	int dev = findCudaDevice(argc, (const char **)argv);

	// allocate host memory:

	float * hA = new float [ SIZE ];
	float * hB = new float [ SIZE ];
	float * hC = new float [ SIZE ];
	unsigned int * hR = new unsigned int [ SIZE/BLOCKSIZE ]; // R for results
	
	for( int i = 0; i < SIZE; i++ )
	{
		hA[i] = Ranf( XCMIN, XCMAX ) ;
		hB[i] = Ranf( YCMIN, YCMAX ) ;
		hC[i] = Ranf( RMIN, RMAX ) ;	
	}
	for ( int i = 0; i < SIZE/BLOCKSIZE; ++i ) {
		hR[i] = 0;
	}

	// allocate device memory:

	float *dA, *dB, *dC;
	unsigned int *dR ;

	dim3 dimsA( SIZE, 1, 1 );
	dim3 dimsB( SIZE, 1, 1 );
	dim3 dimsC( SIZE, 1, 1 );
	dim3 dimsR( SIZE/BLOCKSIZE, 1, 1 );
	//__shared__ float prods[SIZE/BLOCKSIZE];


	cudaError_t status;
	status = cudaMalloc( reinterpret_cast<void **>(&dA), SIZE*sizeof(float) );
	checkCudaErrors( status );
	status = cudaMalloc( reinterpret_cast<void **>(&dB), SIZE*sizeof(float) );
	checkCudaErrors( status );
	status = cudaMalloc( reinterpret_cast<void **>(&dC), (SIZE)*sizeof(float) );
	checkCudaErrors( status );
	status = cudaMalloc( reinterpret_cast<void **>(&dR), (SIZE/BLOCKSIZE)*sizeof(unsigned int));
	checkCudaErrors( status );
	// copy host memory to the device:

	status = cudaMemcpy( dA, hA, SIZE*sizeof(float), cudaMemcpyHostToDevice );
	checkCudaErrors( status );
	status = cudaMemcpy( dB, hB, SIZE*sizeof(float), cudaMemcpyHostToDevice );
	checkCudaErrors( status );
	status = cudaMemcpy( dC, hC, SIZE*sizeof(float), cudaMemcpyHostToDevice );
	checkCudaErrors( status );
	// setup the execution parameters:

	dim3 threads(BLOCKSIZE, 1, 1 );
	dim3 grid( SIZE / threads.x, 1, 1 );

	// Create and start timer

	cudaDeviceSynchronize( );

	// allocate CUDA events that we'll use for timing:

	cudaEvent_t start, stop;
	status = cudaEventCreate( &start );
	checkCudaErrors( status );
	status = cudaEventCreate( &stop );
	checkCudaErrors( status );

	// record the start event:

	status = cudaEventRecord( start, NULL );
	checkCudaErrors( status );

	// execute the kernel:
	for( int t = 0; t < NUMTRIALS; t++)
	{
		MonteCarlo<<< grid, threads >>>( dA, dB, dC, dR );
	}
	// record the stop event:

	status = cudaEventRecord( stop, NULL );
	checkCudaErrors( status );

	// wait for the stop event to complete:

	status = cudaEventSynchronize( stop );
	checkCudaErrors( status );

	float msecTotal = 0.0f;
	status = cudaEventElapsedTime( &msecTotal, start, stop );
	checkCudaErrors( status );

	// compute and print the performance

	double secondsTotal = 0.001 * (double)msecTotal;
	double multsPerSecond = (float)SIZE * (float)NUMTRIALS / secondsTotal;
	double megaMultsPerSecond = multsPerSecond / 1000000.;
	fprintf( stderr, "Array Size = %10d, MegaMultReductions/Second = %10.2lf\n", SIZE, megaMultsPerSecond );

	// copy result from the device to the host:

	status = cudaMemcpy( hR, dR, (SIZE/BLOCKSIZE)*sizeof(unsigned int), cudaMemcpyDeviceToHost );
	checkCudaErrors( status );

	// check the sum :

	unsigned int sum = 0;
	for(int i = 0; i < SIZE/BLOCKSIZE; i++ )
	{
		//fprintf(stderr, "hC[%6d] = %10.2f\n", i, hC[i]);
		sum += hR[i];
	}
	fprintf( stderr, "\nsum = %u\n", sum );

	// clean up memory:
	delete [ ] hA;
	delete [ ] hB;
	delete [ ] hC;
	delete [ ] hR;

	status = cudaFree( dA );
	checkCudaErrors( status );
	status = cudaFree( dB );
	checkCudaErrors( status );
	status = cudaFree( dC );
	checkCudaErrors( status );
	status = cudaFree( dR );
	checkCudaErrors( status );

	return 0;
}

float
Ranf( float low, float high ) {
	float r = (float) rand() ;
	float t = r / (float) RAND_MAX ;
	return low + t * ( high - low ) ;
}
