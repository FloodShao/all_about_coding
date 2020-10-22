cmake_minimum_required(VERSION 3.2)
project(gtest_example)
set(CMAKE_CXX_STANDARD 11)


find_package(GTest REQUIRED)

if(NOT GTest_FOUND)
    message("GTest not found!")
endif()

# 对整个project暴露include路径
include_directories(include)

add_subdirectory(src)
add_subdirectory(tests)
