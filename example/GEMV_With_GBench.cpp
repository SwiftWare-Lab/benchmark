//
// Created by kazem on 9/4/24.
//



//
// Created by kazem on 9/4/24.
//

#include <benchmark/benchmark.h>


// multiplying matrix A, mxn, by a vector, nx1, to produce a vector, mx1
// A is stored in column major format
// A is mxn
// x is nx1
// y is mx1
// y = A*x
void GEMV(int m, int n, double *A, double *x, double *y) {
  //#pragma omp parallel
  {
    //#pragma omp parallel for
    for (int i = 0; i < m; i++) {
      double sum = 0;
      for (int j = 0; j < n; j++) {
        sum += A[j * m + i] * x[j];
      }
      y[i] = sum;
    }
  }
}


void GEMVVec(int m, int n, double *A, double *x, double *y) {
  //#pragma omp parallel
  {
    //#pragma omp parallel for
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
  }
}

// google benchmark for GEMV
static void BM_GEMV(benchmark::State &state) {
  int m = state.range(0);
  int n = state.range(1);
  double *A = new double[m * n];
  double *x = new double[n];
  double *y = new double[m];
  for (int i = 0; i < m * n; i++)
    A[i] = 1.0;
  for (int i = 0; i < n; i++)
    x[i] = 1.0;
  for (auto _ : state) {
    GEMV(m, n, A, x, y);
  }
  delete[] A;
  delete[] x;
  delete[] y;
}

static void BM_GEMVVec(benchmark::State &state) {
  int m = state.range(0);
  int n = state.range(1);
  double *A = new double[m * n];
  double *x = new double[n];
  double *y = new double[m];
  for (int i = 0; i < m * n; i++)
    A[i] = 1.0;
  for (int i = 0; i < n; i++)
    x[i] = 1.0;
  for (auto _ : state) {
    GEMVVec(m, n, A, x, y);
  }
  delete[] A;
  delete[] x;
  delete[] y;
}

BENCHMARK(BM_GEMV)->Args({64, 64})->Args({128, 128})->Args({256, 256})->Args({512, 512})->Args({1024, 1024});
BENCHMARK(BM_GEMVVec)->Args({64, 64})->Args({128, 128})->Args({256, 256})->Args({512, 512})->Args({1024, 1024});



BENCHMARK_MAIN();
