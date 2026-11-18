cmake_minimum_required(VERSION 4.4)
project(AZA_Subset_Sum)

set(CMAKE_CXX_STANDARD 17)

add_executable(AZA_Subset_Sum
        experiments.cpp
        baseline.cpp
        improved.cpp
)
