//
// Created by kazem on 2023-04-13.
//

#ifndef BENCHMARK_CORE_H
#define BENCHMARK_CORE_H

#include <vector>
#include <chrono>
#include <cassert>
#include <iostream>
#include <fstream>
#include <map>
namespace swiftware{
 namespace benchmark{
#define SWBENCH_WITH_PAPI

  /// Measuring time of a kernel
  struct Timer {
   std::vector<std::pair<double,std::string>> ElapsedTimeArray; // for multiple regions of a run
   std::chrono::time_point<std::chrono::system_clock> StartTime, EndTime;
   std::chrono::duration<double> ElapsedSeconds;
   bool IsRunning = false; // make sure we don't start a timer that is already running

   Timer() {}
   ~Timer()= default;
   /// Start the timer
   void start();

   /// Stop the timer
   double stop(std::string RegionName);

   /// Reset the timer
   void reset ();

   /// Print the time in csv format, and add the region name to the header
   std::string printTimeCsv(std::string &Header);
  };


  struct ProflingInfo{
   int NumThreads;
   int NumSubregions;
   int NumEvents;
   /// PCV[i][j][k] = i'th subregion, j'th thread, k'th event
   std::vector<std::vector<std::vector<long long >>> PerformanceCounterValues;
   std::vector<int> EventSet; // Event set for each subregion

   ProflingInfo( int NumThreads, int NumSubregions, int NumEvents):
     NumThreads(NumThreads), NumSubregions(NumSubregions), NumEvents(NumEvents){
    PerformanceCounterValues.resize(NumSubregions);
    for (int i = 0; i < NumSubregions; ++i) {
     PerformanceCounterValues[i].resize(NumThreads);
     for (int j = 0; j < NumThreads; ++j) {
      PerformanceCounterValues[i][j].resize(NumEvents);
     }
    }
    EventSet.resize(NumSubregions);
   }
   ~ProflingInfo()= default;

   std::string printCSV(std::string &Header, std::string Sep=","){
    std::string csv;
    for (int i = 0; i < NumSubregions; ++i) {
     for (int j = 0; j < NumThreads; ++j) {
      for (int k = 0; k < NumEvents; ++k) {
       csv += std::to_string(PerformanceCounterValues[i][j][k]) + Sep;
       Header += "Subregion"+std::to_string(i)+"_Thread"+std::to_string(j)+"_Event"+std::to_string(k)+Sep;
      }
     }
    }
    return csv;
   }
  };

  struct Stats{
   // name and general properties of the benchmark
   std::string Name;
   int NumTrials = 7;

   // a vector to store the time/error/profiling of each trial
   std::vector<Timer> AnalysisTime, ExecutorTime;
   std::vector<std::pair<bool,double>> ErrorPerExecute;
   std::vector<ProflingInfo*> ProfilingInfoTrials;

   // a map to store other stats
   std::map<std::string, std::vector<double>> OtherStats;

   void printCSV(bool PrintHeader=true, std::streambuf* Out = std::cout.rdbuf(), std::string Sep=",");

  };


 } // namespace benchmark
} // namespace swiftware



#endif //BENCHMARK_CORE_H
