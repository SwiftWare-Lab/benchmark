CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

list(APPEND CMAKE_MODULE_PATH
        ${CMAKE_CURRENT_LIST_DIR}/cmake
        "${CMAKE_CURRENT_LIST_DIR}/cmake/third_party"
        )
include(DownloadProject)

# set top level variable
set(SW_BENCHMARK_TOP_LEVEL ON CACHE BOOL "Suppressing benchmark's tests" FORCE)
set(GOOGLE_BENCHMARK_REQUIRED OFF CACHE BOOL "GBench is needed" FORCE)
set(PROFILING_WITH_PAPI ON CACHE BOOL "PAPI is needed" FORCE)
set(PROFILING_WITH_LIKWID OFF CACHE BOOL "LIKWID is needed" FORCE)


project(swbench C CXX)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# set the c++ standard to c++11
set( CMAKE_CXX_STANDARD 11 )

find_package( OpenMP REQUIRED)
set(sw_benchmark_LIBRARIES
        OpenMP::OpenMP_CXX
        )

if (PROFILING_WITH_LIKWID)
    message(STATUS "Profiling with LIKWID is enabled")
    set(LIKWID_DIR "" CACHE PATH "Path likwid performance monitoring and benchmarking suite")
    find_package(likwid REQUIRED)
    set(sw_benchmark_LIBRARIES
            ${sw_benchmark_LIBRARIES}
            ${likwid_LIBRARIES}
            )
    set(sw_benchmark_INCLUDE_DIRS
            ${sw_benchmark_INCLUDE_DIRS}
            ${likwid_INCLUDE_DIRS}
            )
    add_compile_definitions(PROF_WITH_LIKWID)
elseif (PROFILING_WITH_PAPI)
    message(STATUS "Profiling with PAPI is enabled")
    find_package(PAPI REQUIRED)
    set(sw_benchmark_LIBRARIES
            ${sw_benchmark_LIBRARIES}
            ${PAPI_LIBRARIES}
            )
    set(sw_benchmark_INCLUDE_DIRS
            ${sw_benchmark_INCLUDE_DIRS}
            ${PAPI_INCLUDE_DIRS}
            )
    add_compile_definitions(PROF_WITH_PAPI)
    set(sw_benchmark_SRC_FILES
            ${sw_benchmark_SRC_FILES}
            ${CMAKE_CURRENT_LIST_DIR}/src/papi_wrapper.cpp
            )

endif ()

# TODO: google benchmark is not used yet, might be used in the future
find_package(benchmark OPTIONAL_COMPONENTS)
if(NOT TARGET benchmark::benchmark AND GOOGLE_BENCHMARK_REQUIRED)
    message(STATUS "benchmark not found, try to download it")
    include(benchmark)
    set(sw_benchmark_LIBRARIES
            ${sw_benchmark_LIBRARIES}
            benchmark::benchmark
            )
endif()



set_source_files_properties(include/papi_wrapper.cpp PROPERTIES LANGUAGE CXX )

set(sw_benchmark_INCLUDE_DIRS
        ${sw_benchmark_INCLUDE_DIRS}
        ${CMAKE_CURRENT_LIST_DIR}/include
        )
set(sw_benchmark_SRC_FILES
        ${sw_benchmark_SRC_FILES}
        ${CMAKE_CURRENT_LIST_DIR}/src/Timer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/Stats.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/ProfilingInfo.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/SWBench.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/SWTensorBench.cpp
        )



add_library(swbench
        ${sw_benchmark_SRC_FILES}
        )
target_include_directories(swbench PUBLIC
        ${sw_benchmark_INCLUDE_DIRS}
        )
target_link_libraries(swbench PRIVATE
        ${sw_benchmark_LIBRARIES}
        )
if (PROFILING_WITH_PAPI)
    #target_compile_definitions(swbench PRIVATE PW_MULTITHREAD)
    add_compile_definitions(PW_MULTITHREAD)
    #target_compile_options(swbench PRIVATE "-fopenmp")
    add_compile_options("-fopenmp")
endif ()
add_library(swbench::swbench ALIAS swbench)

if(SW_BENCHMARK_TOP_LEVEL)
    add_subdirectory(
            example)
endif()