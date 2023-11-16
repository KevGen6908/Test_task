cmake_minimum_required(VERSION 3.25)
project(TestCase)

set(CMAKE_CXX_STANDARD 17)

add_executable(TestCase
        main.cpp
        ViterbiCodec.cpp
        ViterbiCodec.h

)