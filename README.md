# benchmark
This a benchmark for measuring the performance and profiling C/C++ code. 
For profiling, it is based on [PAPI counters](). 

## Requirements
* CMake
* C/C++ compiler
* PAPI library

## Build

```bash
mkdir build
cd build
cmake ..
make
```

## Run
```bash 
./benchmark
```

## Gettings started

### Adding the benchmark to your code base
You will first need to add the benchmark to your code base. You can add the repo 
to your cmakefile similar to the example provided in [here]().

### Adding a new benchmark
You will first need to set up your benchmark