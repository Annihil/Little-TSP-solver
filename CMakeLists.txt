cmake_minimum_required(VERSION 3.3)
project(Little)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -DDEBUG")

set(SOURCE_FILES main.cpp Little/Little.h Matrix/Matrix.h Matrix/IndexOutOfBoundsException.h
        Matrix/NegativeDimensionException.h TSPLIB/tsplib.cpp TSPLIB/tsplib.h ArgsParser/ArgsParser.cpp
        ArgsParser/ArgsParser.h)
add_executable(Little ${SOURCE_FILES})