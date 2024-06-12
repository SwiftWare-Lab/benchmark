//
// Created by kazem on 2023-04-14.
//

#include "Timer.h"
#include <cassert>
#include <sstream>

namespace swiftware{
 namespace benchmark{

  Timer::Timer() {
   IsRunning = false;
   ElapsedSeconds = std::chrono::duration<double>(0);
#ifdef GPU_ENABLED
   cudaEventCreate(&StartGpuTime);
   cudaEventCreate(&StopGpuTime);
#endif
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

  Timer::~Timer() {
#ifdef GPU_ENABLED
   cudaEventDestroy(StartGpuTime);
   cudaEventDestroy(StopGpuTime);
#endif
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

#ifdef GPU_ENABLED
  void Timer::startGPU(){
   if (IsRunning) {
    std::cout << "Timer is already running" << std::endl;
    assert(false);
   }
    cudaEventRecord(StartGpuTime);
   IsRunning = true;
  }

  double Timer::stopGPU(std::string RegionName=""){
    if (!IsRunning) {
      std::cout << "Timer is not running" << std::endl;
      assert(false);
    }
    cudaEventRecord(StopGpuTime);
    cudaEventSynchronize(StopGpuTime);
    float elapsed = 0;
    cudaEventElapsedTime(&elapsed, StartGpuTime, StopGpuTime);
    double elapsedDouble = (double) elapsed;
    ElapsedTimeArray.push_back(std::make_pair(elapsedDouble,RegionName));
    IsRunning = false;
    return elapsed;
  }
#endif

  void Timer::reset (){
   ElapsedTimeArray.clear();
   IsRunning = false;
  }

  std::string Timer::printTimeCsvHeader(std::string Name, int TrialNo, std::string Sep) {
   std::string header;
   for (int i = 0; i < ElapsedTimeArray.size(); i++) {
    std::string n1 = Name != "" ? Name : ElapsedTimeArray[i].second ;
    header += "Trial" + std::to_string(TrialNo)
              + " Subregion" + std::to_string(i) + " " + n1
              +  Sep;
   }
   return header;
  }

  template <typename T>
  std::string toStringWithPrecision(const T AValue, const int N = 9){
   std::ostringstream out;
   out.precision(N);
   out << std::fixed << AValue;
   return std::move(out).str();
  }

  std::string Timer::printTimeCsv(int TrialNo, std::string Sep) {
   std::string csv;
   for (int i = 0; i < ElapsedTimeArray.size(); i++) {
    csv += toStringWithPrecision<double>(ElapsedTimeArray[i].first, 9) + Sep;
   }
   return csv;
  }

 } // namespace benchmark
} // namespace swiftware