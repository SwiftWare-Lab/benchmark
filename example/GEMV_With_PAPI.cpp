//
// Created by kazem on 2023-04-14.
//
#include "SWTensorBench.h"

#include <omp.h>
#include "papi_wrapper.h"

using namespace swiftware::benchmark;

// multiplying matrix A, mxn, by a vector, nx1, to produce a vector, mx1
// A is stored in column major format
// A is mxn
// x is nx1
// y is mx1
// y = A*x
void GEMV(int m, int n, double *A, double *x, double *y) {
#pragma omp parallel
 {
  pw_start_instruments_loop(omp_get_thread_num());
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
  t.stop("GEMV Sequential");
  return t;
 }

public:
 GEMVWithPAPI(Inputs<double> *In1, Stats *Stat1) : SWTensorBench<double>(In1, Stat1)
   {}

   ~GEMVWithPAPI(){
   }
};

int main(int argc, char *argv[]) {
 auto *in = new Inputs<double>();
 in->ExpName = "GEMV_With_PAPI";
 in->NumThreads = 1;
 in->Dim1 = 1000;
 in->Dim2 = 1000;
 in->Dim3 = 1;
 in->Dim4 = 1;
 in->CorrectSol = new double[in->Dim1];
 // generate a random matrix
 in->A = new double[in->Dim1 * in->Dim2];
 for (int i = 0; i < in->Dim1 * in->Dim2; ++i) {
  in->A[i] = (double) rand() / RAND_MAX;
 }
 // generate a random vector
 in->x = new double[in->Dim2];
 for (int i = 0; i < in->Dim2; ++i) {
  in->x[i] = (double) rand() / RAND_MAX;
 }
 in->y = new double[in->Dim1];

 auto *st = new Stats();
 for (int i = 0; i < in->Dim1; ++i) {
  in->CorrectSol[i] = 0;
 }
 in->Threshold = 1e-6;
 in->NumTrials = 7;
 auto *st1 = new Stats();
 GEMVWithPAPI *gemv = new GEMVWithPAPI(in, st);
 gemv->run();
 gemv->printStats();
 delete gemv;
 return 0;
}
