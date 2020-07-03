cmake_minimum_required(VERSION 2.8)
project(Chess)

file (GLOB_RECURSE FILES_EXE "*.cpp" ".c" "*.h" "*.hpp")

include_directories(include)

add_executable(chess ${FILES_EXE})