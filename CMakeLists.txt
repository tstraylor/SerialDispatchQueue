cmake_minimum_required(VERSION 3.6)
project(serialque)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    queuetest.cpp
    SerialDispatchQueue.cpp
    SerialDispatchQueue.h)

add_executable(serialque ${SOURCE_FILES})