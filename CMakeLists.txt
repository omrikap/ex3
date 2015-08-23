cmake_minimum_required(VERSION 3.2)
project(ex3)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

set(SOURCE_FILES MyString.c MyString.h MyStringMain.c)

add_executable(ex3 ${SOURCE_FILES})