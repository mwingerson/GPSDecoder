cmake_minimum_required(VERSION 2.8)
project( GPSDecoder )

add_library(GPSDecoder GPSDecoder.cpp)

add_executable( testGPSDecoder main.cpp )

target_link_libraries( testGPSDecoder GPSDecoder serial pthread )
