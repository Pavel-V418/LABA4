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
        src/streams/sequence_input_stream.h
        src/streams/istream.h
        src/generators/skip_generator.h
        src/utils/pair.h
        src/generators/zip_generator.h
        src/streams/sequence_output_stream.h
        src/streams/input_stream.h
        src/streams/output_stream.h
        src/streams/file_input_stream.h
        src/streams/file_output_stream.h
        src/streams/string_input_stream.h
        src/streams/string_output_stream.h
        src/generators/append_generator.h
        src/generators/prepend_generator.h
        src/generators/insert_at_generator.h
        src/sliding_cache.h
        src/online_statistic.h
)
