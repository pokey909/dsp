cmake_minimum_required(VERSION 3.15)
project(dsp)

set(CMAKE_CXX_STANDARD 17)

add_executable(dsp main.cpp DSPNode.cpp DSPNode.hpp types.hpp Lowpass.hpp OscNoise.hpp)