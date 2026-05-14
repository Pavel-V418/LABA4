cmake_minimum_required(VERSION 4.2)
project(LABA4)

set(CMAKE_CXX_STANDARD 20)

add_executable(LABA4 main.cpp
        lazy_sequence.h
        generator.h
        rule_generator.h)
