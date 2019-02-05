cmake_minimum_required(VERSION 3.13)
project(wmi)

set(CMAKE_CXX_STANDARD 17)


add_library(wmi library.cpp library.h)

add_executable(wmi-test main.cpp)