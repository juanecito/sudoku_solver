
cmake_minimum_required (VERSION 3.0)

###############################################################################
## file globbing ##############################################################
###############################################################################

# these instructions search the directory tree when cmake is
# invoked and put all files that match the pattern in the variables 
# `sources`
file(GLOB_RECURSE sources src/*.cpp)

# you can use set(sources src/main.cpp) etc if you don't want to
# use globing to find files automatically

project (Sudoku_Solver)

include_directories(${CMAKE_SOURCE_DIR}/include) 

add_executable(sudoku_solver ${sources})


# just for example add some compiler flags
target_compile_options(sudoku_solver PUBLIC -std=c++1y -O3 -funroll-loops -Wall)