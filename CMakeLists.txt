cmake_minimum_required(VERSION 3.10)
project(wmi)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
EXECUTE_PROCESS(COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE)


if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../Release/${ARCHITECTURE})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../Release/${ARCHITECTURE})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../Release/${ARCHITECTURE})

    message(STATUS "Architecture: ${ARCHITECTURE}")

    configure_file(
            ${CMAKE_CURRENT_SOURCE_DIR}/library.h
            ${CMAKE_BINARY_DIR}/../Release/wmi.h
            COPYONLY
    )
endif ()


add_library(wmi library.cpp library.h)


add_executable(wmi-test main.cpp)