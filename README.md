# benchmark
This a benchmark for measuring the performance and profiling C/C++ code. 
For profiling, it is based on [PAPI counters](https://icl.utk.edu/papi/). 

## Requirements
* CMake
* C/C++ compiler
* PAPI library is required for profiling, i.e., when `PROFILING_WITH_PAPI` 
   is set to `ON` in `CMakeLists.txt`. PAPI needs paranoid mode to be smaller
   or equal to 2. You can check the current value with 
   `cat /proc/sys/kernel/perf_event_paranoid`. And you can set it to 2 with
    `sudo bash -c "echo 2 > /proc/sys/kernel/perf_event_paranoid"`.
* OpenMP is required for profiling, i.e., when `PROFILING_WITH_PAPI` 
   is set to `ON` in `CMakeLists.txt`.

## Clone
```bash
git clone https://github.com/SwiftWare-Lab/benchmark.git
```

## Build
Pick one of the following options to build the benchmark.
### Without profiling
After installing the requirements, you can build the benchmark with the:
```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

### With profiling
And to build with profiling support:
```bash 
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DPROFILING_WITH_PAPI=ON ..
make
```

### With profiling for Niagara
**For Niagara**, a script is provided to build the benchmark. You can use it as:
```bash
./build_bench.sh
```
The script should work for other linux machines as well.

## Using benchmark for performance evaluation
Here we will show how to use the benchmark in your code as a library.

### Adding the benchmark to your code base
You will first need to add the benchmark to your code base. You can add 
the repo to your cmakefile similar to the example provided in 
[here](https://github.com/SwiftWare-Lab/practice).

### Enabling profiling mode
Then you can follow the example provided in gemvWithPapi.cpp to measure 
the performance of different implementations of gemv. 

### Analyzing the results
The output is stored as CSV. You need to write a Python script to analyze
the results. 

### Updating the counter list
The list of counters is specified in 
[include/papi_counters.list](include/papi_counters.list). Each line 
specifies a counter. You can use `//` to comment out a line. 