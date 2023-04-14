CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

list(APPEND CMAKE_MODULE_PATH
        ${CMAKE_CURRENT_LIST_DIR}/cmake
        "${CMAKE_CURRENT_LIST_DIR}/cmake/third_party"
        )
include(DownloadProject)
#download_project(PROJ                googlebenchmark
#        GIT_REPOSITORY
#        GIT_TAG         master
#        ${UPDATE_DISCONNECTED_IF_AVAILABLE}
#        )
#add_subdirectory(${googlebenchmark_SOURCE_DIR} ${googlebenchmark_BINARY_DIR})
#
#include_directories("${googlebenchmark_SOURCE_DIR}/include")

# set top level variable
set(SW_BENCHMARK_TOP_LEVEL ON CACHE BOOL "Suppressing benchmark's tests" FORCE)


#find_package(benchmark REQUIRED)
if(NOT TARGET benchmark::benchmark)
    message(STATUS "benchmark not found, try to download it")
    include(benchmark)
#    message(FATAL_ERROR "benchmark not found")
endif()

project(sw_benchmark C CXX)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# set the c++ standard to c++11
set( CMAKE_CXX_STANDARD 11 )

# fin the openmp library
find_package( OpenMP REQUIRED )
find_package(PAPI REQUIRED)


set_source_files_properties(include/papi_wrapper.c PROPERTIES LANGUAGE CXX )

set(sw_benchmark_INCLUDE_DIRS
        ${CMAKE_CURRENT_LIST_DIR}/include)
set(sw_benchmark_SRC_FILES
        ${CMAKE_CURRENT_LIST_DIR}/src/core.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/SWBench.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/SWTensorBench.cpp
        ${CMAKE_CURRENT_LIST_DIR}/include/papi_wrapper.c
        )
set(sw_benchmark_LIBRARIES
        benchmark::benchmark
        OpenMP::OpenMP_CXX
        ${PAPI_LIBRARIES}
        )


add_library(sw_benchmark
        ${sw_benchmark_SRC_FILES}
        )
target_include_directories(sw_benchmark PRIVATE
        ${sw_benchmark_INCLUDE_DIRS}
        )
target_link_libraries(sw_benchmark PRIVATE
        ${sw_benchmark_LIBRARIES}
        )

target_compile_definitions(sw_benchmark PRIVATE PW_MULTITHREAD)
target_compile_options(sw_benchmark PRIVATE "-fopenmp")

if(SW_BENCHMARK_TOP_LEVEL)
    add_subdirectory(
            example)
endif()