CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

list(APPEND CMAKE_MODULE_PATH
        ${CMAKE_CURRENT_LIST_DIR}/cmake
        "${CMAKE_CURRENT_LIST_DIR}/cmake/third_party"
        )
include(DownloadProject)

get_directory_property(HAS_PARENT PARENT_DIRECTORY)
if(HAS_PARENT)
    set(SW_BENCHMARK_TOP_LEVEL OFF)
else()
    set(SW_BENCHMARK_TOP_LEVEL ON)
endif()

# set top level variable
set(GOOGLE_BENCHMARK_REQUIRED OFF CACHE BOOL "GBench is needed")
set(PROFILING_WITH_PAPI OFF CACHE BOOL "PAPI is needed")
set(PROFILING_WITH_LIKWID OFF CACHE BOOL "LIKWID is needed")


project(swbench C CXX)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# set the c++ standard to c++11
set( CMAKE_CXX_STANDARD 11 )

find_package( OpenMP OPTIONAL_COMPONENTS)
if(OpenMP_FOUND)
    message(STATUS "OpenMP is enabled")
    add_compile_definitions(OPENMP_ENABLED)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(sw_benchmark_LIBRARIES
            OpenMP::OpenMP_CXX
            )
endif()


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
    #add_compile_definitions(PROF_WITH_PAPI)
    add_definitions(-DPROF_WITH_PAPI)
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

find_package(CUDA OPTIONAL_COMPONENTS)
find_package(CUDAToolkit OPTIONAL_COMPONENTS)

if (CUDA_FOUND AND CUDAToolkit_FOUND)
    message(STATUS "CUDA found: ${CUDA_VERSION}")
    enable_language(CUDA)
    set(PROJECT_LANGUAGES CXX CUDA)
    add_definitions(-DSWB_GPU_ENABLED)
    message(STATUS "SWB_GPU_ENABLED is defined")
endif()

#set_source_files_properties(include/papi_wrapper.cpp PROPERTIES LANGUAGE CXX )

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
if (CUDA_FOUND AND CUDAToolkit_FOUND)
    target_compile_definitions(swbench PUBLIC SWB_GPU_ENABLED)
    target_link_libraries(swbench PRIVATE
            CUDA::cudart
            )
endif()

if (PROFILING_WITH_PAPI)
    #target_compile_definitions(swbench PRIVATE PW_MULTITHREAD)
    add_compile_definitions(PW_MULTITHREAD)
    #target_compile_options(swbench PRIVATE "-fopenmp")
    add_compile_options("-fopenmp")
endif ()
# if counter_file is defined, add it to the compile definitions

if (DEFINED COUNTER_FILE)
    message(STATUS "Counter file is defined: ${COUNTER_FILE}")
    add_definitions(-DPAPI_FILE_LIST="${COUNTER_FILE}")
endif ()
add_library(swbench::swbench ALIAS swbench)

if(SW_BENCHMARK_TOP_LEVEL)
    add_subdirectory(
            example)
endif()

