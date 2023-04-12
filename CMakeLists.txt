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

#find_package(benchmark REQUIRED)
if(NOT TARGET benchmark::benchmark)
    message(STATUS "benchmark not found, try to download it")
    include(benchmark)
#    message(FATAL_ERROR "benchmark not found")
endif()

project(bench1)
# define c++11
set(CMAKE_CXX_STANDARD 11)

add_executable(bench1
        main.cpp)
target_link_libraries(bench1 benchmark::benchmark)
