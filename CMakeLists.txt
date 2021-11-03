# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5)

# Set the project name
project (crafting-interpreters)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

# Add an executable
add_executable(interpreter *.cpp)