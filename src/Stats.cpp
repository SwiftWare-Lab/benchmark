//
// Created by kazem on 2023-04-14.
//
#include "Stats.h"
#include <iostream>
#include <sstream>

namespace swiftware{
namespace benchmark{

Stats::Stats(std::string Name, std::string OpName, int NumTrials):
            Name(Name), OperationName(OpName), NumTrials(NumTrials){
  ProfilingInfoTrials.resize(NumTrials);
  for (int i = 0; i < NumTrials; ++i) {
    ProfilingInfoTrials[i] = new ProfilingInfo();
  }
}

std::string Stats::printCSVHeader(std::string Sep) {
  std::ostringstream os;
  std::string pHeader="", tHeader="", analysisHeader="";
  assert(NumTrials>=1);
  assert(ProfilingInfoTrials.size()>=1);
    os <<"Operation Name"<<Sep<<"Implementation Name"<<Sep<<"Number of Trials"<<Sep;
    os <<"Matrix Name"<<Sep<<"Number of Threads"<<Sep<<"Number of Subregions"<<Sep;
    analysisHeader = ProfilingInfoTrials[0]->AnalysisTime.printTimeCsvHeader(Name, 0);
    for (int i = 0; i < NumTrials; ++i) {
      //tHeader += "Trial" + std::to_string(i) + "_Time" + Sep;
      //pHeader += ProfilingInfoTrials[i]->printCSVHeader(Sep);
      tHeader = ProfilingInfoTrials[i]->ExecutorTime.printTimeCsvHeader(
          "Executor", i);
      pHeader += ProfilingInfoTrials[i]->printCSVHeader("", i);
      auto err = "Correct" + std::to_string(i) + Sep + "Error" + std::to_string(i);
      os << analysisHeader << tHeader << pHeader << err << Sep;
    }
    for (auto &i : OtherStats) {
      for (int j = 0; j < i.second.size(); ++j) {
        os << i.first +std::to_string(j)<< Sep;
      }
    }
    //os << std::endl;
  return os.str();
}


std::string Stats::printCSV(std::string Sep) {
  std::ostringstream os;
  std::string pCSV="", tCSV="", analysisCSV="";
  assert(NumTrials>=1);
  assert(ProfilingInfoTrials.size()>=1);
  os << OperationName << Sep<< Name<< Sep << NumTrials << Sep;
  os << MatrixName << Sep << ProfilingInfoTrials[0]->NumThreads <<
      Sep << ProfilingInfoTrials[0]->NumSubregions << Sep;
  analysisCSV = ProfilingInfoTrials[0]->AnalysisTime.printTimeCsv(0);
  os << analysisCSV;
  for (int i = 0; i < NumTrials; ++i) {
    //tHeader += "Trial" + std::to_string(i) + "_Time" + Sep;
    //pHeader += ProfilingInfoTrials[i]->printCSVHeader(Sep);
    tCSV = ProfilingInfoTrials[i]->ExecutorTime.printTimeCsv(i);
    pCSV += ProfilingInfoTrials[i]->printCSV( i);
    auto err = std::to_string(ProfilingInfoTrials[i]->ErrorPerExecute.first)+
        Sep+std::to_string(ProfilingInfoTrials[i]->ErrorPerExecute.second);
    os << tCSV << pCSV << err << Sep;
  }
  for (auto &i : OtherStats) {
    for (int j = 0; j < i.second.size(); ++j) {
      os << i.second[j]<< Sep;
    }
  }
  //os << std::endl;
  return os.str();
}

} // namespace benchmark
} // namespace swiftware
