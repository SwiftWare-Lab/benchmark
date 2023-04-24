//
// Created by kazem on 2023-04-14.
//
#include "GEMV_With_PAPI_Utils.h"


int main(int argc, char *argv[]) {
  auto *in = new Inputs<double>(1000, 1000, 0, 0);
  in->ExpName = "GEMV_With_PAPI";
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
  in->NumTrials = 2;

  auto *st = new Stats( "GEMV_Parallel", "MV", in->NumTrials);
  for (int i = 0; i < in->Dim1; ++i) {
    in->CorrectSol[i] = 0;
  }
  in->Threshold = 1e-6;
  auto *gemv = new GEMVWithPAPI(in, st);
  gemv->run();
  auto headerStat = gemv->printStatsHeader();
  auto baselineStat = gemv->printStats();
  delete gemv;

  auto *gemvVec = new GEMVVecWithPAPI(in, st);
  gemvVec->run();
  auto gemvVecStat = gemvVec->printStats();
  delete gemvVec;

  auto inHeader = in->printCSVHeader();
  auto inStat = in->printCSV();
  delete in;
  delete st;

  /// Exporting the results
  std::cout<<headerStat<<inHeader<<std::endl;
  std::cout<<baselineStat<<inStat<<std::endl;
  std::cout<<gemvVecStat<<inStat<<std::endl;

  return 0;
}
