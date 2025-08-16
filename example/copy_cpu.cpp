//
// Created by kazem on 8/15/25.
//


#include "benchmark/benchmark.h"

#include <iostream>

void copy_func_1(double *A, int m, int REP) {
  for(int rep=0; rep < REP; rep++) {
    for(int a=0; a <  m ; a++) {
      A[a] = A[a] + 1;
    }
  }
}

void copy_func_2(double *A, int m, int REP) {
  auto mask = (1<<m) - 1;
  for(auto rep=0; rep < REP; rep++) {
      auto addr = ((rep + 523)*253573) & mask;
      A[addr] = A[addr] + 1;
  }
}

static void BM_COPY(benchmark::State &state) {
  auto m = state.range(0);
  auto REP = state.range(1);
  double *A = new double[m]();

  for (auto _: state) {
    copy_func_1(A, m, REP);
  }
  delete[] A;
  state.SetItemsProcessed(state.iterations() * m);
  state.SetComplexityN(m);
  state.SetBytesProcessed(state.iterations() * m * REP * sizeof(double));
  // std::cout << "Total bytes processed (KB): "
  //           << m * sizeof(double)/1024 << ", " << state.iterations() << std::endl;



}

static void BM_COPY2(benchmark::State &state) {
  auto m = state.range(0);
  auto REP = state.range(1);
  double *A = new double[m]();

  for (auto _: state) {
    copy_func_2(A, m, REP);
  }
  delete[] A;
  state.SetItemsProcessed(state.iterations() * m);
  state.SetComplexityN(m);
  state.SetBytesProcessed(state.iterations() * m * REP * sizeof(double));
  // add input args
  state.counters["m"] = m;
  state.counters["REP"] = REP;

  //state.Co

}

//BENCHMARK(BM_COPY)->Args({{1000, 1000}})->Unit(benchmark::kMillisecond);
BENCHMARK(BM_COPY)->ArgsProduct({benchmark::CreateRange(256, 2*4096*1024, /*multi=*/2), {1}})
    ->Unit(benchmark::kMicrosecond);

BENCHMARK(BM_COPY2)->ArgsProduct({benchmark::CreateRange(256, 2*4096*1024, /*multi=*/2), {1}})
    ->Unit(benchmark::kMicrosecond);

//BENCHMARK_MAIN();



// Main function to run the benchmarks.
int main(int argc, char** argv) {
  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
  return 0;
}