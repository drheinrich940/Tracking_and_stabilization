cmake_minimum_required(VERSION 3.8)
project(OpenCV)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -Wextra -ansi -pedantic")

find_package(OpenCV REQUIRED)

set(SOURCE_FILES cap_test.cpp harris_detection_contours.cpp)

#set(SOURCE_FILES harris_detection_contours.cpp)

add_executable(OpenCV ${SOURCE_FILES})
#add_executable(OpenCV harris_detection_contours.cpp)


include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(OpenCV ${OpenCV_LIBS})
