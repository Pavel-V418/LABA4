cmake_minimum_required(VERSION 4.2)
project(LABA4)

set(CMAKE_CXX_STANDARD 20)

add_executable(LABA4 src/main.cpp
        src/lazy_sequence.h
        src/generators/generator.h
        src/generators/rule_generator.h
        src/generators/map_generator.h
        src/generators/filter_generator.h
        src/generators/take_generator.h
        src/generators/concat_generator.h
        src/cardinal.h
        src/streams/read_only_stream.h
        src/streams/istream.h
        src/generators/skip_generator.h
        src/utils/pair.h
        src/generators/zip_generator.h
        src/streams/write_only_stream.h
)
