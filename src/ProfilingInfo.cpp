//
// Created by kazem on 2023-04-14.
//

#include <papi.h>
#include "ProfilingInfo.h"

namespace swiftware{
 namespace benchmark{


//   ProflingInfo( int NumThreads, int NumSubregions, int NumEvents):
//     NumThreads(NumThreads), NumSubregions(NumSubregions), NumEvents(NumEvents){
//    PerformanceCounterValues.resize(NumSubregions);
//    for (int i = 0; i < NumSubregions; ++i) {
//     PerformanceCounterValues[i].resize(NumThreads);
//     for (int j = 0; j < NumThreads; ++j) {
//      PerformanceCounterValues[i][j].resize(NumEvents);
//     }
//    }
//    EventSet.resize(NumSubregions);
//   }


   void ProfilingInfo::resizeValueArray(int NumThreadsIn, int NumSubregionsIn, int NumEventsIn){
    NumThreads = NumThreadsIn;
    NumSubregions = NumSubregionsIn;
    NumEvents = NumEventsIn;
    PerformanceCounterValues.resize(NumSubregions);
    for (int i = 0; i < NumSubregions; ++i) {
     PerformanceCounterValues[i].resize(NumThreads);
     for (int j = 0; j < NumThreads; ++j) {
      PerformanceCounterValues[i][j].resize(NumEvents);
     }
    }
    EventSet.resize(NumSubregions);
   }

  std::string convertCodeToString(int Code){
#ifdef PROF_WITH_PAPI
   char eventCodeStr[PAPI_MAX_STR_LEN];
   PAPI_event_code_to_name(Code,eventCodeStr);
   std::string ret(eventCodeStr);
#else
   std::string ret="";
#endif
   return ret;
  }

   std::string ProfilingInfo::printCSV(int TrialNo, std::string &Header, std::string Sep){
    std::string csv;
    for (int i = 0; i < NumSubregions; ++i) {
     for (int j = 0; j < NumThreads; ++j) {
      for (int k = 0; k < NumEvents; ++k) {
       csv += std::to_string(PerformanceCounterValues[i][j][k]) + Sep;
       Header += "Trial" + std::to_string(TrialNo)+"_Subregion"+std::to_string(i)+
         "_Thread"+std::to_string(j)//+"_Event"+std::to_string(k)
         +"_"+convertCodeToString(EventSet[k])
         + Sep;
      }
     }
    }
    return csv;
   }



 }
}