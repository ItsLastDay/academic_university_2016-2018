#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.h>
#include "cl.hpp"

#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

using matrix_value_t = float;
using matrix_t = vector<matrix_value_t>;

matrix_t read_matrix(size_t size)
{
    // Read square matrix from stdin.
    matrix_t res(size * size);
    size_t idx = 0;
    for (size_t i = 0; i < size; i++) 
    {
        for (size_t j = 0; j < size; j++) {
            cin >> res[idx];
            ++idx;
        }
    }

    return res;
}


int main() {
    vector<cl::Platform> platforms;
    vector<cl::Device> devices;
    vector<cl::Kernel> kernels;

    try 
    {
        cl::Platform::get(&platforms);
        platforms[0].getDevices(CL_DEVICE_TYPE_GPU, &devices);
        cl::Context context(devices);
        cl::CommandQueue queue(context, devices[0]);

        ifstream cl_file("2dconv.cl");
        string cl_string(istreambuf_iterator<char>(cl_file), (istreambuf_iterator<char>()));
        cl::Program::Sources source(1, 
                make_pair(cl_string.c_str(), cl_string.length() + 1));
        cl::Program program(context, source);

        const size_t block_size = 16;
        program.build(devices, "-D BLOCK_SIZE=16");

        size_t N, M;
        cin >> N >> M;

        auto initial_mat = read_matrix(N);
        auto convolution_kernel_mat = read_matrix(M);
        matrix_t resulting_mat(N * N, 0);

        size_t num_big_matrix_bytes = sizeof(matrix_value_t) * N * N;
        size_t num_small_matrix_bytes = sizeof(matrix_value_t) * M * M;


        cl::Buffer dev_initial_mat(context, CL_MEM_READ_ONLY, num_big_matrix_bytes);
        cl::Buffer dev_convolution_kernel_mat(context, CL_MEM_READ_ONLY, 
                num_small_matrix_bytes);
        cl::Buffer dev_resulting_mat(context, CL_MEM_WRITE_ONLY, num_big_matrix_bytes);

        queue.enqueueWriteBuffer(dev_initial_mat, CL_TRUE, 0, num_big_matrix_bytes,
                initial_mat.data());
        queue.enqueueWriteBuffer(dev_convolution_kernel_mat, CL_TRUE, 0, 
                num_small_matrix_bytes, convolution_kernel_mat.data());

        size_t ndrange_size = block_size;
        while (ndrange_size < N)
            ndrange_size <<= 1;

        cl::Kernel kernel(program, "convolution_2d");
        cl::KernelFunctor conv_2d(kernel, queue, cl::NullRange,
               cl::NDRange(ndrange_size, ndrange_size), 
               cl::NDRange(block_size, block_size)); 

        conv_2d(dev_initial_mat, dev_convolution_kernel_mat, dev_resulting_mat,
                (int)N, (int)(M - 1) / 2);

        queue.enqueueReadBuffer(dev_convolution_kernel_mat, CL_TRUE, 0, 
                num_big_matrix_bytes, resulting_mat.data());

        for (size_t i = 0; i < N; i++)
        {
            for (size_t j = 0; j < N; j++) 
            {
                size_t idx = i * N + j;
                cout << setprecision(3) << resulting_mat[idx] << " ";
            }
            cout << endl;
        }
    } 
    catch (cl::Error e) 
    {
        cout << endl << e.what() << " : " << e.err() << endl;
    }
}

