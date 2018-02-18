EXECUTABLES = 2dconv

all: $(EXECUTABLES)

ifdef OPENCL_INC
  CL_CFLAGS = -I$(OPENCL_INC)
endif

ifdef OPENCL_LIB
  CL_LDFLAGS = -L$(OPENCL_LIB)
endif

2dconv: 2dconv.cpp 2dconv.cl
	g++ $(CL_CFLAGS) $(CL_LDFLAGS) -std=c++0x -o$@ $< -lrt -lOpenCL

clean:
	rm -f $(EXECUTABLES) *.o
