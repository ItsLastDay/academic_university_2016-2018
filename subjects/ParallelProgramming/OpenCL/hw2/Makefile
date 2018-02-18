EXECUTABLES = prefix

all: $(EXECUTABLES)

ifdef OPENCL_INC
  CL_CFLAGS = -I$(OPENCL_INC)
endif

ifdef OPENCL_LIB
  CL_LDFLAGS = -L$(OPENCL_LIB)
endif

FLAGS=-Wall -Wextra -pedantic -std=c++14 -O0 -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wcast-qual -Wcast-align -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fstack-protector -ggdb #-fsanitize=address

prefix: prefix.cpp prefix.cl
	g++ $(CL_CFLAGS) $(CL_LDFLAGS) $(FLAGS) -o$@ $< -lrt -lOpenCL

clean:
	rm -f $(EXECUTABLES) *.o
