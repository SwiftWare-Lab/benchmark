//
// Created by kazem on 2023-04-14.
//

#ifndef SW_BENCHMARK_SWBENCH_H
#define SW_BENCHMARK_SWBENCH_H

#include "Stats.h"

#include <algorithm>

namespace swiftware{
namespace benchmark{


// Step 1: Create a custom reporter class that inherits from BenchmarkReporter.
// class MedianReporter : public benchmark::BenchmarkReporter {
// public:
//   bool ReportContext(const Context& context) override {
//     std::cout << "--------------------------------------------------------\n";
//     std::cout << "Benchmark           Time           CPU Time        Median\n";
//     std::cout << "--------------------------------------------------------\n";
//     return true;
//   }
//
//   void ReportRuns(const std::vector<Run>& report) override {
//     if (report.empty()) return;
//
//     // Collect real_time values for median calculation
//     std::vector<double> times;
//     for (const auto& run : report) {
//       times.push_back(run.real_accumulated_time);
//     }
//     std::sort(times.begin(), times.end());
//     double median = 0.0;
//     size_t n = times.size();
//     if (n > 0) {
//       if (n % 2 == 0)
//         median = 0.5 * (times[n / 2 - 1] + times[n / 2]);
//       else
//         median = times[n / 2];
//     }
//
//     // Print the first run's info and the computed median
//     const auto& run = report.front();
//     std::cout << run.benchmark_name() << "    "
//               << run.real_accumulated_time << "    "
//               << run.cpu_accumulated_time << "    "
//               << median << "\n";
//   }
//
//   void Finalize() override {
//     std::cout << "--------------------------------------------------------\n";
//   }
// };
//


class SWBench{
protected:
  Stats *St;



  virtual void setup(){
  }

  virtual Timer analysis(){
    return Timer();
  }

  virtual void preExecute(){
  }

  virtual Timer execute(){
    return Timer();
  }

  virtual int collectProfilingInfo(int TrialNo = 0);
  virtual int enableProfiling();

  virtual bool verify(double &Error){
    return true;
  }

  virtual void teardown(){
  }

public:

  SWBench(Stats *St1);
  ~SWBench();

  /*
    * The main function to run the benchmark, shows the overall
   */
  void run();

  virtual std::string printStatsHeader(){
    return St->printCSVHeader();
  }
  virtual std::string printStats(){
    return St->printCSV();
  }
};


} // namespace benchmark
} // namespace swiftware

#endif //SW_BENCHMARK_SWBENCH_H
