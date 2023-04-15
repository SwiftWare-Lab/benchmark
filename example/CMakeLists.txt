

set(EXAMPLES_DIR ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "EXAMPLES_DIR: ${EXAMPLES_DIR}")

include_directories(${sw_benchmark_INCLUDE_DIRS})

project(examples)

add_executable(gemvwithPAPI  ${EXAMPLES_DIR}/GEMV_With_PAPI.cpp)
target_link_libraries(gemvwithPAPI sw_benchmark )
target_compile_definitions(gemvwithPAPI PRIVATE PW_MULTITHREAD)
target_compile_options(gemvwithPAPI PRIVATE "-fopenmp")
