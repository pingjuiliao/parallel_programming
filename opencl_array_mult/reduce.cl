kernel
void
ArrayMultReduce( global const float *dA, global const float *dB, global float *dC, local float *prod )
{
	int gid = get_global_id( 0 );     
    int lsize = get_local_size( 0 );
    int lid = get_local_id( 0 );
    int groupid   = get_group_id( 0 );

    prod[lid] = dA[gid] * dB[gid];

    for( int offset = 1; offset < lsize; offset *= 2)
    {
    	int mask = 2*offset - 1;
        barrier( CLK_LOCAL_MEM_FENCE );
        if ((lid & mask) == 0) {
            prod[lid] += prod[lid + offset];
        }     
  
    }
   
    barrier(CLK_LOCAL_MEM_FENCE);
    if (lid == 0)
    	dC[groupid] = prod[0];

}