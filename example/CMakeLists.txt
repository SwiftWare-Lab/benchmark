

set(EXAMPLES_DIR ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "EXAMPLES_DIR: ${EXAMPLES_DIR}")

include_directories(${sw_benchmark_INCLUDE_DIRS})

project(examples)

    add_executable(gemvwithPAPI  ${EXAMPLES_DIR}/GEMV_With_PAPI.cpp)
    target_link_libraries(gemvwithPAPI swbench::swbench )


if(PROFILING_WITH_LIKWID)
    add_executable(gemvwithLIKWID  ${EXAMPLES_DIR}/GEMV_With_Likwid.cpp)
    target_link_libraries(gemvwithLIKWID swbench::swbench )
endif ()

    add_executable(gemvWithGBench
            ${EXAMPLES_DIR}/GEMV_With_GBench.cpp)
    target_link_libraries(gemvWithGBench benchmark::benchmark )