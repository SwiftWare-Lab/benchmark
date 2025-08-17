//
// Created by kazem on 8/16/25.
//

#ifndef SWBENCH_TEST_KERNELS_H
#define SWBENCH_TEST_KERNELS_H
namespace swiftware::benchmark{
    // multiplying matrix A, mxn, by a vector, nx1, to produce a vector, mx1
    // A is stored in column major format
    // A is mxn
    // x is nx1
    // y is mx1
    // y = A*x
    void GEMV(int m, int n, double *A, double *x, double *y) {
#pragma omp parallel
      {
#pragma omp parallel for
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
#pragma omp parallel
      {
#pragma omp parallel for
        for (int i = 0; i < m; i++) {
          double sum = 0;
          int j = 0;
          for (; j <= n - 4; j += 4) {
            sum += A[j * m + i] * x[j];
            sum += A[(j + 1) * m + i] * x[j + 1];
            sum += A[(j + 2) * m + i] * x[j + 2];
            sum += A[(j + 3) * m + i] * x[j + 3];
          }
          for (; j < n; j++) {
            sum += A[j * m + i] * x[j];
          }
          y[i] = sum;
        }
      }
    }
  }
#endif // SWBENCH_TEST_KERNELS_H
