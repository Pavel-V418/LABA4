cmake_minimum_required(VERSION 4.2)
project(LABA4)

set(CMAKE_CXX_STANDARD 20)

add_executable(LABA4 src/main.cpp
        src/lazy/lazy_sequence.h
        src/generators/generator.h
        src/generators/rule_generator.h
        src/generators/map_generator.h
        src/generators/filter_generator.h
        src/generators/take_generator.h
        src/generators/concat_generator.h
        src/lazy/cardinal.h
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
        src/lazy/sliding_cache.h
        src/statistics/online_statistics.h
        src/statistics/sliding_median.h
        src/generators/stream_number_generator.h
        src/statistics/statistics_processor.h
        src/generators/fibonacci_generator.h
        src/generators/random_generator.h
)
