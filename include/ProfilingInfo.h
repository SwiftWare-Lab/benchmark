//
// Created by kazem on 2023-04-14.
//

#ifndef SW_BENCHMARK_PROFILINGINFO_H
#define SW_BENCHMARK_PROFILINGINFO_H

#include "Timer.h"
#include <vector>
#include <string>


namespace swiftware{
 namespace benchmark{


  struct ProfilingInfo{
   int NumThreads;
   int NumSubregions;
   int NumEvents;
   /// PCV[i][j][k] = i'th subregion, j'th thread, k'th event
   std::vector<std::vector<std::vector<long long >>> PerformanceCounterValues;
   std::vector<int> EventSet; // Event set for each subregion of the executor
   std::vector<std::string> EventSetStr; // event set for likwid

   Timer AnalysisTime, ExecutorTime;
   std::pair<bool,double> ErrorPerExecute;

   ProfilingInfo()= default;

   ~ProfilingInfo()= default;

   void resizeValueArray(int NumThreadsIn, int NumSubregionsIn, int NumEventsIn);

   std::string printCSV(int TrialNo, std::string &Header, std::string Sep=",");

  };

 } // namespace benchmark
} // namespace swiftware

#endif //SW_BENCHMARK_PROFILINGINFO_H
