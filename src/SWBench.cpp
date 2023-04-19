//
// Created by kazem on 2023-04-14.
//

#include <omp.h>
#include "SWBench.h"

#ifdef PROF_WITH_PAPI
#include "papi_wrapper.h"
#endif
#ifdef PROF_WITH_LIKWID
#include <likwid.h>
#endif

namespace swiftware{
namespace benchmark{
SWBench::SWBench(Stats *St1):St(St1){
}

SWBench::~SWBench(){
  //delete St;
}

void SWBench::run(){
  double error;
  setup();
  auto ta = analysis();
  for (int i = 0; i < St->NumTrials; ++i) {
    St->ProfilingInfoTrials[i]->AnalysisTime = ta; // same inspector time for all trials
    preExecute();
    enableProfiling();
    auto te = execute();
    St->ProfilingInfoTrials[i]->ExecutorTime = te;
    collectProfilingInfo(i);
    auto flag = verify(error);
    St->ProfilingInfoTrials[i]->ErrorPerExecute = std::make_pair(flag,error);
  }
  teardown();
}

int SWBench::enableProfiling(){
//#ifdef PROF_WITH_PAPI
//  pw_init_instruments;
//#endif
#ifdef PROF_WITH_LIKWID
  int i, j;
  int err;
  int* cpus;
  int gid;
  double result = 0.0;
  char estr[] = "UOPS_EXECUTED_TOTAL_CYCLES:PMC0,L2_TRANS_L2_WB:PMC1";
  char* enames[2] = {"UOPS_EXECUTED_TOTAL_CYCLES:PMC0","L2_TRANS_L2_WB:PMC1"};
  int n = sizeof(enames) / sizeof(enames[0]);
  //perfmon_setVerbosity(3);
  // Load the topology module and print some values.
  err = topology_init();
  if (err < 0)
  {
    printf("Failed to initialize LIKWID's topology module\n");
    return 1;
  }
  // CpuInfo_t contains global information like name, CPU family, ...

  St->CpuInfo = get_cpuInfo();
  // CpuTopology_t contains information about the topology of the CPUs.
  St->CpuTopo = get_cpuTopology();
  auto *info = St->CpuInfo;
  auto *topo = St->CpuTopo;
  // Create affinity domains. Commonly only needed when reading Uncore counters
  affinity_init();

  printf("Likwid example on a %s with %d CPUs\n", info->name, topo->numHWThreads);

  cpus = (int*)malloc(topo->numHWThreads * sizeof(int));
  if (!cpus)
    return 1;

  for (i=0;i<topo->numHWThreads;i++)
  {
    cpus[i] = topo->threadPool[i].apicId;
  }

  // Must be called before perfmon_init() but only if you want to use another
  // access mode as the pre-configured one. For direct access (0) you have to
  // be root.
  //accessClient_setaccessmode(0);

  // Initialize the perfmon module.
  err = perfmon_init(topo->numHWThreads, cpus);
  if (err < 0)
  {
    printf("Failed to initialize LIKWID's performance monitoring module\n");
    topology_finalize();
    return 1;
  }

  // Add eventset string to the perfmon module.
  gid = perfmon_addEventSet(estr);
  if (gid < 0)
  {
    printf("Failed to add event string %s to LIKWID's performance monitoring module\n", estr);
    perfmon_finalize();
    topology_finalize();
    return 1;
  }

  // Setup the eventset identified by group ID (gid).
  err = perfmon_setupCounters(gid);
  if (err < 0)
  {
    printf("Failed to setup group %d in LIKWID's performance monitoring module\n", gid);
    perfmon_finalize();
    topology_finalize();
    return 1;
  }
  // Start all counters in the previously set up event set.
  err = perfmon_startCounters();
  if (err < 0)
  {
    printf("Failed to start counters for group %d for thread %d\n",gid, (-1*err)-1);
    perfmon_finalize();
    topology_finalize();
    return 1;
  }
#endif
  return 0;
}

int SWBench::collectProfilingInfo(int TrialNo){
#ifdef PROF_WITH_LIKWID
  int i, j;
  int err;
  int* cpus;
  int gid;
  double result = 0.0;
  char estr[] = "L2_LINES_IN_ALL:PMC0,L2_TRANS_L2_WB:PMC1";
  char* enames[2] = {"L2_LINES_IN_ALL:PMC0","L2_TRANS_L2_WB:PMC1"};
  int n = sizeof(enames) / sizeof(enames[0]);

  auto *info = St->CpuInfo;
  auto *topo = St->CpuTopo;
  printf("Likwid example on a %s with %d CPUs\n", info->name, topo->numHWThreads);
  auto *cpu = St->Cpus;
//  cpus = (int*)malloc(topo->numHWThreads * sizeof(int));
//  if (!cpus)
//    return 1;
//
//  for (i=0;i<topo->numHWThreads;i++)
//  {
//    cpus[i] = topo->threadPool[i].apicId;
//  }
  err = perfmon_readCounters();
  if (err < 0)
  {
    printf("Failed to read counters for group %d for thread %d\n",gid, (-1*err)-1);
    perfmon_finalize();
    topology_finalize();
    return 1;
  }

  // Print the result of every thread/CPU for all events in estr, counting from last read/startCounters().
  printf("Work task 1/2 measurements:\n");
  for (j=0; j<n; j++)
  {
    for (i = 0;i < topo->numHWThreads; i++)
    {
      result = perfmon_getLastResult(gid, j, i);
      printf("- event set %s at CPU %d: %f\n", enames[j], cpus[i], result);
    }
  }
#endif

#ifdef PROF_WITH_PAPI
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
      auto *pi = St->ProfilingInfoTrials[TrialNo]; //new ProflingInfo(pwNthreads, 1, nEvents);
      pi->resizeValueArray(pwNthreads, 1, nEvents);
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
  St->ProfilingInfoTrials[TrialNo] = pi;
  // pw_close();
#endif //SWBENCH_WITH_PAPI
  return 0;
}


} // namespace benchmark
} // namespace swiftware