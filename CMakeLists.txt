cmake_minimum_required(VERSION 3.26)
project(TestCase)

set(CMAKE_CXX_STANDARD 17)


#find_package(FFTW REQUIRED)
include_directories(${FFTW_INCLUDE_DIRS})

add_executable(TestCase
        main.cpp
        FastFourierTransform.cpp
        FastFourierTransform.h
        ViterbiCodec.cpp
        ViterbiCodec.h

)

target_link_libraries(TestCase PRIVATE fftw3)

