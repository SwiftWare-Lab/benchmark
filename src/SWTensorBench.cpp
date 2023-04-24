//
// Created by kazem on 2023-04-14.
//

#include "SWTensorBench.h"
namespace swiftware{
namespace benchmark{

template <class T> std::string Inputs<T>::printCSVHeader(std::string Sep){
  std::string retValue = "NumTrials,NumThreads,Dim1,Dim2,Dim3,Dim4,Threshold,ExperimentName,";
  return retValue;
}

template <class T> std::string Inputs<T>::printCSV(std::string Sep){
  std::string retValue = std::to_string(NumTrials) + Sep + std::to_string(NumThreads) + Sep +
                         std::to_string(Dim1) + Sep + std::to_string(Dim2) + Sep +
                         std::to_string(Dim3) + Sep + std::to_string(Dim4) + Sep +
                         std::to_string(Threshold) + Sep + ExpName + Sep;
  return retValue;
}

template std::string Inputs<double>::printCSVHeader(std::string Sep);
template std::string Inputs<double>::printCSV(std::string Sep);
template std::string Inputs<float>::printCSVHeader(std::string Sep);
template std::string Inputs<float>::printCSV(std::string Sep);


}//end namespace benchmark
}//end namespace swiftware
