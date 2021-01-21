cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_COMPILER "g++")

project(jnp1_z6)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-O2 -Wall -Wextra -Wpedantic -Werror)

add_subdirectory(ooasm)

include(CTest)
add_subdirectory(tests)
