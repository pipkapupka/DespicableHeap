cmake_minimum_required(VERSION 4.0)
project(DespicableHeap CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(DespicableHeap INTERFACE)

target_include_directories(DespicableHeap INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

enable_testing()
add_subdirectory(tests)