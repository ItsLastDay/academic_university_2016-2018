__kernel void convolution_2d(__global float* initial_mat, __global float* kernel_mat,
                                      __global float* resulting_mat, int n, int hm)
{
    int row = get_global_id(0);
    int col = get_global_id(1);
    
    if (row >= n || col >= n)
        return;

    float sum = 0;

    for (int k = -hm; k <= hm; ++k)
    {
        for (int l = -hm; l <= hm; ++l)
        {
            int idx_init = (row + k) * n + col + l;
            int idx_kernel = (k + hm) * (hm * 2 + 1) + l + hm;
            if (row + k >= 0 && row + k < n && col + l >= 0 && col + l < n)
                sum += initial_mat[idx_init] * kernel_mat[idx_kernel]; 
        }
    }

    resulting_mat[row * n + col] = sum;
}
