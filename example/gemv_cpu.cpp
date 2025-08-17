//
// Created by kazem on 8/16/25.
//

#include "benchmark/benchmark.h"
#include "test_kernels.h"

#include <iostream>


static void BM_GEMV_BASELINE(benchmark::State &state) {
  auto m = state.range(0);
  auto n = state.range(1);
  double *A = new double[m*n]();
  std::fill_n(A, m*n, 1e-5);
  auto *x = new double[n]();
  std::fill_n(x, n, 1.0);
  auto *y = new double[m]();

  for (auto _: state) {
    swiftware::benchmark::GEMV(m, n, A, x, y);
  }
  state.SetItemsProcessed(state.iterations() * m * n);
  state.SetBytesProcessed(state.iterations() * m * n * sizeof(double) + m + n);
  // verify
  for (int i = 0; i < m; i++) {
    if (std::abs(y[i] - (n*1e-5)) > 1e-6) {
      std::cerr << "Error in GEMV: y[" << i << "] = " << y[i] << " != " << n << std::endl;
    }
  }
  delete[] x;
  delete[] y;
  delete[] A;
}

static void BM_GEMV_VECTORIZED(benchmark::State &state) {
  auto m = state.range(0);
  auto n = state.range(1);
  double *A = new double[m*n]();
  std::fill_n(A, m*n, 1e-5);
  auto *x = new double[n]();
  std::fill_n(x, n, 1.0);
  auto *y = new double[m]();

  for (auto _: state) {
    swiftware::benchmark::GEMVVec(m, n, A, x, y);
  }
  state.SetItemsProcessed(state.iterations() * m * n);
  state.SetBytesProcessed(state.iterations() * m * n * sizeof(double) + m + n);
  // verify
  for (int i = 0; i < m; i++) {
    if (std::abs(y[i] - (n*1e-5)) > 1e-6) {
      std::cerr << "Error in GEMV: y[" << i << "] = " << y[i] << " != " << n*1e-5 << std::endl;
    }
  }
  delete[] x;
  delete[] y;
  delete[] A;
}

BENCHMARK(BM_GEMV_BASELINE)->ArgsProduct({benchmark::CreateRange(256, 16*1024, /*multi=*/2), {1024}})
    ->Unit(benchmark::kMicrosecond)->Iterations(1)->Repetitions(50);

BENCHMARK(BM_GEMV_VECTORIZED)->ArgsProduct({benchmark::CreateRange(256, 16*1024, /*multi=*/2), {1024}})
    ->Unit(benchmark::kMicrosecond)->Iterations(1)->Repetitions(50);


BENCHMARK_MAIN();