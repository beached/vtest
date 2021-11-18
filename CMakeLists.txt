# Copyright (c) Darrell Wright
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Official repository: https://github.com/beached/gumbo_pp
#

cmake_minimum_required(VERSION 3.20)

project("vtest"
        VERSION "0.1.0"
        LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17 CACHE STRING "The C++ standard whose features are requested.")

option(DAW_ENABLE_TESTING "Build tests and examples" OFF)

set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
find_package(benchmark CONFIG REQUIRED)

add_subdirectory(extern)

add_executable(vtest1 vtest1.cpp)
target_link_libraries(vtest1 PRIVATE benchmark::benchmark benchmark::benchmark_main daw::daw-header-libraries)
