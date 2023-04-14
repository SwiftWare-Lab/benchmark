//
// Created by kazem on 2023-04-14.
//
#include "core.h"
#include <iostream>

namespace swiftware{
 namespace benchmark{

  void Stats::printCSV(bool PrintHeader, std::streambuf *Out, std::string Sep) {
   std::ostream os(Out);
   std::string pHeader="", pCSV="";
   if (PrintHeader) {
    os << "Name"<<Sep<<"Number of Trials"<<Sep;
    for (auto &i : OtherStats) {
     os << i.first << Sep;
    }
    pCSV = ProfilingInfoTrials[0]->printCSV(pHeader);
    os << pHeader << std::endl;
    os << std::endl;
   }
   os << Name << Sep << NumTrials << Sep;
   for (auto &i : OtherStats) {
    os << i.second[0] << ",";
   }
   os << pCSV << std::endl;
   os << std::endl;
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

  std::string Timer::printTimeCsv(std::string &Header) {
   std::string csv;
   for (auto &i : ElapsedTimeArray) {
    csv += std::to_string(i.first) + ",";
    Header += i.second + ",";
   }
   return csv;
  }

 } // namespace benchmark
} // namespace swiftware
