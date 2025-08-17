

set(EXAMPLES_DIR ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "EXAMPLES_DIR: ${EXAMPLES_DIR}")

include_directories(${sw_benchmark_INCLUDE_DIRS})

project(examples)

add_executable(gemv_cpu
        ${EXAMPLES_DIR}/gemv_cpu.cpp
)
target_link_libraries(gemv_cpu
        swbench::swbench
        benchmark::benchmark
)
