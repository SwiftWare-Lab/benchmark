CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

list(APPEND CMAKE_MODULE_PATH
        ${CMAKE_CURRENT_LIST_DIR}/cmake
        "${CMAKE_CURRENT_LIST_DIR}/cmake/third_party"
        )
include(DownloadProject)

# set top level variable
set(SW_BENCHMARK_TOP_LEVEL ON CACHE BOOL "Suppressing benchmark's tests" FORCE)
set(GOOGLE_BENCHMARK_REQUIRED OFF CACHE BOOL "GBench is needed" FORCE)



project(sw_benchmark C CXX)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# set the c++ standard to c++11
set( CMAKE_CXX_STANDARD 11 )

find_package( OpenMP REQUIRED)
find_package(PAPI REQUIRED)
set(sw_benchmark_LIBRARIES
        OpenMP::OpenMP_CXX
        ${PAPI_LIBRARIES}
        )
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
        ${CMAKE_CURRENT_LIST_DIR}/include)
set(sw_benchmark_SRC_FILES
        ${CMAKE_CURRENT_LIST_DIR}/src/Timer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/Stats.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/ProfilingInfo.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/SWBench.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/SWTensorBench.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/papi_wrapper.cpp
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