
//
// Created by kazem on 24/04/23.
//

#include "SWTensorBench.h"
#include "papi_wrapper.h"
#include <omp.h>

#ifndef SW_BENCHMARK_GEMV_WITH_PAPI_UTILS_H
#define SW_BENCHMARK_GEMV_WITH_PAPI_UTILS_H

using namespace swiftware::benchmark;

// multiplying matrix A, mxn, by a vector, nx1, to produce a vector, mx1
// A is stored in column major format
// A is mxn
// x is nx1
// y is mx1
// y = A*x
void GEMV(int m, int n, double *A, double *x, double *y) {
  pw_init_instruments;
#pragma omp parallel
  {
    pw_start_instruments_loop(omp_get_thread_num());
#pragma omp parallel for
    for (int i = 0; i < m; i++) {
      double sum = 0;
      for (int j = 0; j < n; j++) {
        sum += A[j * m + i] * x[j];
      }
      y[i] = sum;
    }
    pw_stop_instruments_loop(omp_get_thread_num());
  }
}
void GEMVVec(int m, int n, double *A, double *x, double *y) {
  pw_init_instruments;
#pragma omp parallel
  {
    pw_start_instruments_loop(omp_get_thread_num());
#pragma omp parallel for
    for (int i = 0; i < m; i++) {
      double sum = 0;
      for (int j = 0; j < n/4; j+=4) {
        sum += A[j * m + i] * x[j];
        sum += A[(j+1) * m + i] * x[j+1];
        sum += A[(j+2) * m + i] * x[j+2];
        sum += A[(j+3) * m + i] * x[j+3];
      }
      for(int j = n/4*4; j < n; j++)
        sum += A[j * m + i] * x[j];
      y[i] = sum;
    }
    pw_stop_instruments_loop(omp_get_thread_num());
  }
}

class GEMVWithPAPI : public SWTensorBench<double> {
protected:
  void setup() override {
    Out = new Outputs<double>();
    Out->Out = new double[In->Dim1]();
  }

  void preExecute() override {
    for (int i = 0; i < In->Dim1; ++i) {
      In->CorrectSol[i] = 0;
      Out->Out[i] = 0;
    }
  }

  Timer execute() override {
    Timer t;
    t.start();
    GEMV(In->Dim1, In->Dim2, In->A, In->x, In->y);
    t.stop();
    return t;
  }

public:
  GEMVWithPAPI(Inputs<double> *In1, Stats *Stat1) : SWTensorBench<double>(In1, Stat1)
  {}

  ~GEMVWithPAPI(){
  }
};

class GEMVVecWithPAPI : public GEMVWithPAPI {
protected:
  Timer execute() override {
    Timer t;
    t.start();
    GEMVVec(In->Dim1, In->Dim2, In->A, In->x, In->y);
    t.stop();
    return t;
  }
public:
  GEMVVecWithPAPI(Inputs<double> *In1, Stats *Stat1) : GEMVWithPAPI(In1, Stat1)
  {}
};


#endif // SW_BENCHMARK_GEMV_WITH_PAPI_UTILS_H

