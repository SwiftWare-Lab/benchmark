//
// Created by kazem on 2023-04-14.
//

#include <omp.h>
#include "SWBench.h"

#ifdef SWBENCH_WITH_PAPI
#include "papi_wrapper.h"
#endif

namespace swiftware{
 namespace benchmark{
  SWBench::SWBench(){
   St = new Stats();
  }

  SWBench::~SWBench(){
   delete St;
  }

  void SWBench::run(){
   double error;
   setup();
   auto ta = analysis();
   St->AnalysisTime.push_back(ta);
   for (int i = 0; i < St->NumTrials; ++i) {
    preExecute();
    enableProfiling();
    auto te = execute();
    St->ExecutorTime.push_back(te);
    collectProfilingInfo();
    auto flag = verify(error);
    St->ErrorPerExecute.push_back(std::make_pair(flag,error));
   }
   teardown();
  }

  void SWBench::enableProfiling(){
#ifdef SWBENCH_WITH_PAPI
   pw_init_instruments;
#endif
  }

  void SWBench::collectProfilingInfo(){
#ifdef SWBENCH_WITH_PAPI

#if defined(_OPENMP)
#    if !defined(PW_MULTITHREAD)
   #        pragma omp parallel
    {
        if (omp_get_thread_num() == pw_counters_threadid)
        {
#    endif
#endif // _OPENMP
#if defined(PW_MULTITHREAD)
   int pwNthreads = 1;
#    pragma omp parallel
   {
#    pragma omp master
    {
     pwNthreads = omp_get_num_threads();
    }
   }

   int pwNthread = 0, nEvents=0;
   // counting the number of events
   for (nEvents = 0; PW_EVTLST(__pw_nthread, nEvents) != 0;
        ++nEvents);
   auto *pi = new ProflingInfo(pwNthreads, 1, nEvents);
#    pragma omp for ordered schedule(static, 1)
   for (pwNthread = 0; pwNthread < pwNthreads; ++pwNthread){
    int pwEvid;
    for (pwEvid = 0; PW_EVTLST(__pw_nthread, pwEvid) != 0;
         ++pwEvid){
     //if (verbose) PRINT_OUT("%s=", _pw_eventlist[__pw_evid]);
     pi->EventSet[pwEvid] = pw_eventlist[pwEvid];
     pi->PerformanceCounterValues[0][pwNthread][pwEvid] =
       PW_VALUES(pwNthread, pwEvid);
    }
   }
#    pragma omp barrier
#    pragma omp master
   {
    free(PW_thread);
   }
#else // PW_MULTITHREAD
   #    if defined(PW_CSV)
   PRINT_OUT("%d", pw_counters_threadid);
#    else
   PRINT_OUT("PAPI thread %2d\t", pw_counters_threadid);
#    endif
   for (__pw_evid = 0; pw_eventlist[__pw_evid] != 0; ++__pw_evid)
   {
    if (verbose) PRINT_OUT("%s=", _pw_eventlist[__pw_evid]);
    PRINT_OUT("%s%llu", PW_CSV_SEPARATOR, pw_values[__pw_evid]);
    if (verbose) PRINT_OUT("\n");
   }
   PRINT_OUT("\n");
#endif // PW_MULTITHREAD
#if defined(_OPENMP)
#    if !defined(PW_MULTITHREAD)
   }
    }
#        pragma omp barrier
#    endif
#endif
   St->ProfilingInfoTrials.push_back(pi);
   pw_close();
#endif //SWBENCH_WITH_PAPI
  }


 } // namespace benchmark
} // namespace swiftware