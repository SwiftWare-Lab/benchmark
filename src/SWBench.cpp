//
// Created by kazem on 2023-04-14.
//

#ifdef OPENMP_ENABLED
#include <omp.h>
#endif
#include "SWBench.h"

#ifdef PROF_WITH_PAPI
#include "papi_wrapper.h"
#endif
#ifdef PROF_WITH_LIKWID
#include <likwid.h>
#endif

namespace swiftware{
namespace benchmark{



} // namespace benchmark
} // namespace swiftware