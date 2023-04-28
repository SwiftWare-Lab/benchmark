# benchmark
This a benchmark for measuring the performance and profiling C/C++ code. 
For profiling, it is based on [PAPI counters](). 

## Requirements
* CMake
* C/C++ compiler
* PAPI library is required for profiling, i.e., when `PROFILING_WITH_PAPI` 
   is set to `ON` in `CMakeLists.txt`. PAPI needs paranoid mode to be smaller
   or equal to 2. You can check the current value with 
   `cat /proc/sys/kernel/perf_event_paranoid`. And you can set it to 2 with
    `sudo bash -c "echo -1 > /proc/sys/kernel/perf_event_paranoid"`.
* OpenMP is required for profiling, i.e., when `PROFILING_WITH_PAPI` 
   is set to `ON` in `CMakeLists.txt`.

## Build

After installing the requirements, you can build the benchmark with the:
```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```


## Using benchmark for performance evaluation
Here we will show how to use the benchmark in your code as a library.

### Adding the benchmark to your code base
You will first need to add the benchmark to your code base. You can add 
the repo to your cmakefile similar to the example provided in [here]().

### Enabling profiling mode

You will first need to set up your benchmark

### Updating the counter list
The list of counters is specified in include/papi_counters.list. Each line 
specifies a counter. You can use `//` to comment out a line. 