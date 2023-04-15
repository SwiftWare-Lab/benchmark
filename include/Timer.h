//
// Created by kazem on 2023-04-14.
//

#ifndef SW_BENCHMARK_TIMER_H
#define SW_BENCHMARK_TIMER_H

#include <iostream>
#include <chrono>
#include <vector>

namespace swiftware{
 namespace benchmark{

  /// Measuring time of a kernel
  struct Timer {
   std::vector<std::pair<double,std::string>> ElapsedTimeArray; // for multiple regions of a run
   std::chrono::time_point<std::chrono::system_clock> StartTime, EndTime;
   std::chrono::duration<double> ElapsedSeconds;
   bool IsRunning = false; // make sure we don't start a timer that is already running

   Timer();

   Timer(const Timer &T);
   ~Timer()= default;
   /// Start the timer
   void start();

   /// Stop the timer
   double stop(std::string RegionName);

   /// Reset the timer
   void reset ();

   /// Print the time in csv format, and add the region name to the header
   std::string printTimeCsv(int TrialNo, std::string &Header);
  };

 } // end namespace benchmark
} // end namespace swiftware

#endif //SW_BENCHMARK_TIMER_H
