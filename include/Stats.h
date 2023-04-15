//
// Created by kazem on 2023-04-13.
//

#ifndef BENCHMARK_CORE_H
#define BENCHMARK_CORE_H

#include "ProfilingInfo.h"
#include <vector>
#include <chrono>
#include <cassert>
#include <iostream>
#include <fstream>
#include <map>
namespace swiftware{
 namespace benchmark{
#define SWBENCH_WITH_PAPI

  struct Stats{
   // name and general properties of the benchmark
   std::string Name, OperationName;
   std::string MatrixName;
   int NumTrials;

   // a vector to store the time/error/profiling of each trial
   std::vector<ProfilingInfo*> ProfilingInfoTrials;

   // a map to store other stats
   std::map<std::string, std::vector<double>> OtherStats;

   void printCSV(bool PrintHeader=true, std::streambuf* Out = std::cout.rdbuf(), std::string Sep=",");

   Stats(std::string Name, int NumTrials);

  };


 } // namespace benchmark
} // namespace swiftware



#endif //BENCHMARK_CORE_H
