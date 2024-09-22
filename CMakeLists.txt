# Define the minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Set the project name and specify that it's a C project
project(LearningC C)

# Add the source files
add_executable(learningC src/main.c)

# Optionally set compiler flags
# set(CMAKE_C_FLAGS "-Wall -Wextra")