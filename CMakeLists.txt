cmake_minimum_required(VERSION 3.8)
project(OpenCV)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -Wextra -ansi -pedantic")

find_package(OpenCV REQUIRED)

set(SOURCE_FILES
        cap_test.cpp)

add_executable(OpenCV ${SOURCE_FILES})

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(OpenCV ${OpenCV_LIBS})
