#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.h>
#include "cl.hpp"

#include <vector>
#include <fstream>
#include <iostream>
#include <iterator>
#include <chrono>
#include <iomanip>
#include <assert.h>

using std::cout;
using std::cin;
using std::endl;
using std::vector;

using value_t = float;
size_t const block_size = 256;


void expand(vector<value_t>& input) 
{
    while (input.size() % block_size)
        input.push_back(0);
}

size_t sz_bytes(size_t sz) {
    return sizeof(value_t) * sz;
}

void propagate_subblocks_results(cl::Context const& context,
        cl::Program const& program,
        cl::CommandQueue& queue,
        vector<value_t> const& blocks_scanned,
        size_t sz,
        cl::Buffer& output) 
{
    cl::Kernel kernel_propagate(program, "propagate_subblock_results");
    cl::make_kernel<cl::Buffer, 
        cl::Buffer> propagate(kernel_propagate);

    cl::Buffer scanned_buffer(context, CL_MEM_READ_ONLY, sz_bytes(blocks_scanned.size()));
    queue.enqueueWriteBuffer(scanned_buffer, CL_TRUE, 0, sz_bytes(blocks_scanned.size()),
            &blocks_scanned[0]);

    cl::EnqueueArgs eargs(queue, 
            cl::NullRange, 
            cl::NDRange(sz), 
            cl::NDRange(block_size));

    cl::Event event = propagate(eargs, 
            output, 
            scanned_buffer);

    event.wait();
}

void run_one_scan(cl::Program const& program,
        cl::CommandQueue& queue,
        size_t sz,
        cl::Buffer& input,
        cl::Buffer& output,
        int need_inplace) 
{
    // https://forums.khronos.org/showthread.php/8317-cl-hpp-KernelFunctor-gone-replaced-with-KernelFunctorGlobal
    cl::Kernel kernel_hs(program, "scan_hillis_steele");
    cl::make_kernel<cl::Buffer, 
        cl::Buffer, 
        cl::LocalSpaceArg, 
        cl::LocalSpaceArg,
        int> scan_hs(kernel_hs);

    cl::EnqueueArgs eargs(queue, 
            cl::NullRange, 
            cl::NDRange(sz), 
            cl::NDRange(block_size));
    cl::Event event = scan_hs(eargs, 
            input, 
            output, 
            cl::Local(sz_bytes(block_size)), 
            cl::Local(sz_bytes(block_size)),
            need_inplace);

    event.wait();
}



vector<value_t> perform_scan(cl::Context const& context,
        cl::Program const& program, 
        cl::CommandQueue& queue,
        vector<value_t> input) 
{
    expand(input);
    size_t sz = input.size();

    cl::Buffer dev_input (context, CL_MEM_READ_ONLY, sz_bytes(sz));
    cl::Buffer dev_output(context, CL_MEM_READ_WRITE, sz_bytes(sz));
    queue.enqueueWriteBuffer(dev_input, CL_TRUE, 0, sz_bytes(sz), &input[0]);

    if (sz <= block_size) {
        run_one_scan(program, queue, sz, dev_input, dev_output, 1);
    } else {
        size_t num_blocks = (sz + block_size - 1) / block_size;
        cl::Buffer block_sums(context, CL_MEM_READ_WRITE, sz_bytes(num_blocks));
        run_one_scan(program, queue, sz, dev_input, block_sums, 0);
        run_one_scan(program, queue, sz, dev_input, dev_output, 1);

        vector<value_t> block_sums_vec(num_blocks, 0);
        queue.enqueueReadBuffer(block_sums, CL_TRUE, 0, sz_bytes(num_blocks),
                &block_sums_vec[0]);

        auto blocks_scan_subresult = perform_scan(context,
                program, queue,
                block_sums_vec);

        propagate_subblocks_results(context, program, queue, blocks_scan_subresult, 
                sz, dev_output);
    }

    vector<value_t> output(sz, 0);
    queue.enqueueReadBuffer(dev_output, CL_TRUE, 0, sizeof(value_t) * sz, &output[0]);
    return output;
}

vector<value_t> read_input() {
    size_t sz;
    cin >> sz;
    vector<value_t> input(sz);
    for (size_t i = 0; i < sz; ++i) {
        cin >> input[i];
    }
    return input;
}


int main()
{
   vector<cl::Platform> platforms;
   vector<cl::Device> devices;
   vector<cl::Kernel> kernels;

   auto input = read_input();

   try {

      cl::Platform::get(&platforms);
      platforms[0].getDevices(CL_DEVICE_TYPE_ALL, &devices);
      cl::Context context(devices);
      cl::CommandQueue queue(context, devices[0], CL_QUEUE_PROFILING_ENABLE);

      std::ifstream cl_file("prefix.cl");
      std::string cl_string(std::istreambuf_iterator<char>(cl_file), (std::istreambuf_iterator<char>()));
      cl::Program::Sources source(1, std::make_pair(cl_string.c_str(),
         cl_string.length() + 1));

      cl::Program program(context, source);
      program.build(devices, "-D BLOCK_SIZE=256");

      // http://en.cppreference.com/w/cpp/chrono/high_resolution_clock/now
      auto start = std::chrono::high_resolution_clock::now();
      auto output = perform_scan(context, program, queue, input);
      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double> diff = end - start;

      // https://stackoverflow.com/a/8554456/5338270
      cout.setf(std::ios::fixed, std::ios::floatfield);
      for (size_t i = 0; i < input.size(); i++)
          cout << std::setprecision(3) << output[i] << " ";
      cout << endl;

      cout << "Time elapsed: " << diff.count() << endl;

   }
   catch (cl::Error e)
   {
      std::cout << std::endl << e.what() << " : " << e.err() << std::endl;
   }

   return 0;
}
