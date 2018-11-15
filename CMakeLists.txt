cmake_minimum_required(VERSION 3.8)
project(PC2)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Constraint.cpp Constraint.h Solver.cpp Solver.h Variable.cpp Variable.h Domains.cpp Domains.h)
add_executable(PC2 ${SOURCE_FILES})