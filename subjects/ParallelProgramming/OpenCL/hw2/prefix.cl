#define SWAP(a,b) {__local int * tmp=a; a=b; b=tmp;}

__kernel void scan_hillis_steele(__global float* input, __global float* output, __local float* a, __local float * b, int need_inplace)
{
    uint gid = get_global_id(0);
    uint lid = get_local_id(0);
    uint block_size = get_local_size(0);
 
    a[lid] = b[lid] = input[gid];
    barrier(CLK_LOCAL_MEM_FENCE);
 
    for(uint s = 1; s < block_size; s <<= 1)
    {
        if(lid > (s-1))
        {
            b[lid] = a[lid] + a[lid-s];
        }
        else
        {
            b[lid] = a[lid];
        }
        barrier(CLK_LOCAL_MEM_FENCE);
        SWAP(a,b);
    }

    if (need_inplace) 
    {
        output[gid] = a[lid];
    } 
    else 
    {
        uint group_id = get_group_id(0);
        if (lid == 255)
            output[group_id] = a[lid];
    }
}


__kernel void propagate_subblock_results(__global float* output,
                                         __global float* block_results)
{
    uint group_id = get_group_id(0);
    uint gid = get_global_id(0);
    if (group_id > 0)
        output[gid] += block_results[group_id - 1];
}
