cmake_minimum_required(VERSION 3.20)

# Compiler options
# Use C++ 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Force UTF-8 on MSVC
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
# Make sure macro "__cplusplus" is correctly defined on MSVC
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/Zc:__cplusplus>")

project(testasio)
find_package(Boost REQUIRED COMPONENTS program_options system)
add_executable(testasio testasio.cpp)
target_link_libraries(testasio PRIVATE Boost::program_options Boost::system)

