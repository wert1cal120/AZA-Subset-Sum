cmake_minimum_required(VERSION 3.16)
project(AZA_Subset_Sum)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(AZA_Subset_Sum
        experiments.cpp
        baseline.cpp
        improved.cpp
)
