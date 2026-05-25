#ifndef LABA4_STRING_INPUT_STREAM_H
#define LABA4_STRING_INPUT_STREAM_H

#include <string>
#include <stdexcept>
#include "input_stream.h"

class StringInputStream : public InputStream<char> {

private:
    std::string source;

    bool opened;
    int position;
    char current_char;

public:

    StringInputStream(const std::string& source)
        : source(source), opened(false), position(0), current_char('\0') {}

    void open() override{
        opened = true;

        position = 0;
    }

    void close() override{
        opened = false;
    }

    bool is_open() const override{
        return opened;
    }

    bool is_end_of_stream() const override{
        return position >= source.size();
    }

    bool can_seek() const override{
        return true;
    }

    bool can_go_back() const override{
        return true;
    }

    int get_position() const override{
        return position;
    }

    void seek(int pos) override{
        if(pos < 0)
            throw std::out_of_range("Negative seek position");

        if(pos > source.size())
            throw std::out_of_range("Seek out of range");

        position = pos;
    }

    const char& read() override{
        if(!opened)
            throw std::logic_error("Stream is closed");

        if(is_end_of_stream())
            throw std::out_of_range("End of stream reached");

        current_char = source[position];

        position++;

        return current_char;
    }
};

#endif //LABA4_STRING_INPUT_STREAM_H