//
// Created by kazem on 2023-04-14.
//

#ifndef SW_BENCHMARK_SWBENCH_H
#define SW_BENCHMARK_SWBENCH_H

#include "core.h"


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

   virtual void collectProfilingInfo();
   virtual void enableProfiling();

   virtual bool verify(double &Error){
    return true;
   }

   virtual void teardown(){
   }

  public:

   SWBench();
   ~SWBench();

   /*
    * The main function to run the benchmark, shows the overall
    */
   void run();

   void printStats(){
    St->printCSV();
   }
  };


  } // namespace benchmark
 } // namespace swiftware

#endif //SW_BENCHMARK_SWBENCH_H
