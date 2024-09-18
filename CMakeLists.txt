cmake_minimum_required(VERSION 3.28)

# set(ENABLE_ASAN ON)
# set(CMAKE_CXX_SCAN_FOR_MODULES 0)

set(CMAKE_CXX_STANDARD 23)

project(ext_any)

include_directories(./include ./include/ext )

add_subdirectory(test)
add_subdirectory(examples)
