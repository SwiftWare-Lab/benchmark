//
// Created by kazem on 2023-04-14.
//

#ifndef SW_BENCHMARK_SWBENCH_H
#define SW_BENCHMARK_SWBENCH_H

#include "Stats.h"


namespace swiftware{
 namespace benchmark{




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
