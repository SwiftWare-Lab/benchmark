#
# Copyright 2020 Adobe. All rights reserved.
# This file is licensed to you under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License. You may obtain a copy
# of the License at http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software distributed under
# the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR REPRESENTATIONS
# OF ANY KIND, either express or implied. See the License for the specific language
# governing permissions and limitations under the License.
#
if(TARGET benchmark::benchmark)
    return()
endif()

#find_package(benchmark OPTIONAL_COMPONENTS)
if(NOT benchmark_FOUND)
    message(STATUS "Third-party (internal): creating target 'benchmark::benchmark'")

    include(FetchContent)
    FetchContent_Declare(
            benchmark
            GIT_REPOSITORY https://github.com/google/benchmark.git
            GIT_TAG        b177433f3ee2513b1075140c723d73ab8901790f
    )

    FetchContent_GetProperties(benchmark)
    if(NOT benchmark_POPULATED)
        FetchContent_Populate(benchmark)
    endif()
    # set to download google test
    set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON CACHE BOOL "Enable downloading dependencies." FORCE)

    add_subdirectory(${benchmark_SOURCE_DIR} ${benchmark_BINARY_DIR} EXCLUDE_FROM_ALL)



endif()