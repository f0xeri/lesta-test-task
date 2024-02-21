cmake_minimum_required(VERSION 3.27)
project(lesta_test)

set(CMAKE_CXX_STANDARD 17)

add_executable(lesta_test main.cpp
        RingBuffer.hpp
        RingBuffer2.hpp)
