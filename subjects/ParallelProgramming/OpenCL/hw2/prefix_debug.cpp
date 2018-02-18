#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.h>
#include "cl.hpp"

#include <vector>
#include <fstream>
#include <iostream>
#include <iterator>
#include <iomanip>
#include <assert.h>

#include <errno.h>
#include <sys/resource.h>
#include <sys/time.h>

using std::cout;
using std::endl;

int main()
{
   std::vector<cl::Platform> platforms;
   std::vector<cl::Device> devices;
   std::vector<cl::Kernel> kernels;

   /*
    * Does not help:
   struct rlimit lim = {
       123456,
       123456
   };
   if(setrlimit(RLIMIT_STACK, &lim)) {
       perror("failed set stack");
       return 1;
   }
   getrlimit(RLIMIT_STACK, &lim);
   cout << lim.rlim_cur << " " << lim.rlim_max << endl;
   */

   try {

      cl::Platform::get(&platforms);
      cout << platforms.size() << endl;
      //https://github.com/marchv/opencl-info/blob/master/opencl-info.cpp#L39
      for (auto& platform: platforms) {
          cout << platform.getInfo<CL_PLATFORM_NAME>() << endl;
      }
      platforms[0].getDevices(CL_DEVICE_TYPE_ALL, &devices);
      cout << devices.size() << endl;
      cl::Context context(devices);
      cl::CommandQueue queue(context, devices[0], CL_QUEUE_PROFILING_ENABLE);

      std::ifstream cl_file("prefix.cl");
      std::string cl_string(std::istreambuf_iterator<char>(cl_file), (std::istreambuf_iterator<char>()));
      cl::Program::Sources source(1, std::make_pair(cl_string.c_str(),
         cl_string.length() + 1));

      cl::Program program(context, source);
      program.build(devices);

      // create a message to send to kernel
      size_t const block_size = 512;
      size_t const test_array_size = 512;
      size_t const output_size = test_array_size;
      std::vector<int> input(test_array_size);
      std::vector<int> output(output_size, 0);
      for (size_t i = 0; i < test_array_size; ++i)
      {
         input[i] = i % 10;
      }

      // allocate device buffer to hold message
      cl::Buffer dev_input (context, CL_MEM_READ_ONLY, sizeof(int) * test_array_size);
      cl::Buffer dev_output(context, CL_MEM_WRITE_ONLY, sizeof(int) * output_size);

      // copy from cpu to gpu
      queue.enqueueWriteBuffer(dev_input, CL_TRUE, 0, sizeof(int) * test_array_size, &input[0]);

      queue.finish();

      // load named kernel from opencl source
      // https://forums.khronos.org/showthread.php/8317-cl-hpp-KernelFunctor-gone-replaced-with-KernelFunctorGlobal
      cl::Kernel kernel_hs(program, "scan_hillis_steele");
      cl::make_kernel<cl::Buffer, cl::Buffer, cl::LocalSpaceArg, cl::LocalSpaceArg> scan_hs(kernel_hs);
      cl::EnqueueArgs eargs(queue, cl::NullRange, cl::NDRange(test_array_size), cl::NDRange(block_size));
      cl::Event event = scan_hs(eargs, 
              dev_input, 
              dev_output, 
              cl::Local(sizeof(int) * block_size), 
              cl::Local(sizeof(int) * block_size));

      event.wait();
      cl_ulong start_time = event.getProfilingInfo<CL_PROFILING_COMMAND_START>();
      cl_ulong end_time   = event.getProfilingInfo<CL_PROFILING_COMMAND_END>();
      cl_ulong elapsed_time = end_time - start_time;

      queue.enqueueReadBuffer(dev_output, CL_TRUE, 0, sizeof(int) * output_size, &output[0]);

      std::cout << std::setprecision(2) << "Total time: " << elapsed_time / 1000000.0 << " ms" << std::endl;

   }
   catch (cl::Error e)
   {
      std::cout << std::endl << e.what() << " : " << e.err() << std::endl;
   }

   return 0;
}
