//
// Created by kazem on 2023-04-14.
//

#include "Timer.h"
#include <cassert>

namespace swiftware{
 namespace benchmark{

  Timer::Timer() {
   IsRunning = false;
   ElapsedSeconds = std::chrono::duration<double>(0);
  }

  Timer::Timer(const Timer &T){
   for (const auto ele : T.ElapsedTimeArray) {
    ElapsedTimeArray.push_back(ele);
   }
   IsRunning = T.IsRunning;
   StartTime = T.StartTime;
   EndTime = T.EndTime;
   ElapsedSeconds = T.ElapsedSeconds;
  }

  void Timer::start() {
   if (IsRunning) {
    std::cout << "Timer is already running" << std::endl;
    assert(false);
   }
   StartTime = std::chrono::system_clock::now();
   IsRunning = true;
  }

  double Timer::stop(std::string RegionName) {
   if (!IsRunning) {
    std::cout << "Timer is not running" << std::endl;
    assert(false);
   }
   EndTime = std::chrono::system_clock::now();
   ElapsedSeconds = EndTime - StartTime;
   ElapsedTimeArray.push_back(std::make_pair(ElapsedSeconds.count(),RegionName));
   IsRunning = false;
   return ElapsedSeconds.count();
  }

  void Timer::reset (){
   ElapsedTimeArray.clear();
   IsRunning = false;
  }

  std::string Timer::printTimeCsv(int TrialNo, std::string &Header) {
   std::string csv;
   for (auto &i : ElapsedTimeArray) {
    csv += std::to_string(i.first) + ",";
    Header += i.second + "_" + std::to_string(TrialNo) + ",";
   }
   return csv;
  }

 } // namespace benchmark
} // namespace swiftware