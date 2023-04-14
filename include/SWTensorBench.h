//
// Created by kazem on 2023-04-14.
//

#ifndef SW_BENCHMARK_SWTENSORBENCH_H
#define SW_BENCHMARK_SWTENSORBENCH_H

#include "SWBench.h"
#include <map>

namespace swiftware{
 namespace benchmark{

  template <class T>
  struct Inputs{
   int NumTrials = 7;
   int NumThreads = 1;
   std::string ExpName="";
   int Dim1, Dim2, Dim3, Dim4;

   T* CorrectSol;
   T Threshold = 1e-6;

   double *A, *x, *y;
  };

  template <class T>
  struct Outputs{
   T* Out;
  };




  /// Base class for all tensor benchmarks
  template <class T>
  class SWTensorBench : public SWBench{
   protected:
   Inputs<T> *In;
   Outputs<T> *Out{};
   Stats *St;


   bool verify(double &Error) override{
    bool retValue = true;
    T infNorm = 0;
    for (int i = 0; i < In->Dim1; ++i) {
     if (std::abs(Out->Out[i] - In->CorrectSol[i]) > infNorm){
      infNorm = std::abs(Out->Out[i] - In->CorrectSol[i]);
     }
    }
    Error = (double) infNorm;
    if (infNorm > In->Threshold){
     retValue = false;
    }
    return retValue;
   }

  public:
   SWTensorBench(Inputs<T> *In1, Stats *St1):
    In(In1), St(St1){}

    ~SWTensorBench() {
    }
  };

 } // namespace benchmark
} // namespace swiftware




#endif //SW_BENCHMARK_SWTENSORBENCH_H
