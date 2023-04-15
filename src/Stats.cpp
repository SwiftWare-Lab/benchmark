//
// Created by kazem on 2023-04-14.
//
#include "Stats.h"
#include <iostream>

namespace swiftware{
 namespace benchmark{

  Stats::Stats(std::string Name, int NumTrials):
  Name(Name), NumTrials(NumTrials){
   ProfilingInfoTrials.resize(NumTrials);
   for (int i = 0; i < NumTrials; ++i) {
    ProfilingInfoTrials[i] = new ProfilingInfo();
   }
  }

  void Stats::printCSV(bool PrintHeader, std::streambuf *Out, std::string Sep) {
   std::ostream os(Out);
   std::string pHeader="", tHeader="", pCSV="", tCSV="";
   if (PrintHeader) {
    os << "Operation Name"<<Sep<<"Number of Trials"<<Sep;
    for (int i = 0; i < NumTrials; ++i) {
     //tHeader += "Trial" + std::to_string(i) + "_Time" + Sep;
     //pHeader += ProfilingInfoTrials[i]->printCSVHeader(Sep);
     tCSV += ProfilingInfoTrials[i]->ExecutorTime.printTimeCsv(i, tHeader);
     pCSV += ProfilingInfoTrials[i]->printCSV(i, pHeader);
     os << tHeader << pHeader << std::endl;
    }

    for (auto &i : OtherStats) {
     os << i.first << Sep;
    }
    os << std::endl;
   }
   os << OperationName << Sep << NumTrials << Sep;
   os << tCSV << pCSV << std::endl;
   for (auto &i : OtherStats) {
    os << i.second[0] << ",";
   }
   os << std::endl;
  }

 } // namespace benchmark
} // namespace swiftware
