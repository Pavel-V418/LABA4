#ifndef LABA4_STRING_OUTPUT_STREAM_H
#define LABA4_STRING_OUTPUT_STREAM_H

#include <string>
#include <stdexcept>
#include "output_stream.h"

class StringOutputStream : public OutputStream<char> {

private:

    std::string buffer; // строка в которую идёт запись

    bool opened;
    int position;

public:

    StringOutputStream()
        : buffer(""), opened(false), position(0) {}

    void open() override{

        opened = true;
        buffer.clear();

        position = 0;
    }

    void close() override{
        opened = false;
    }

    bool is_open() const override{
        return opened;
    }

    int get_position() const override{
        return position;
    }

    void write(const char& item) override{
        if(!opened)
            throw std::logic_error("Stream is closed");

        buffer += item;
        position++;
    }

    const std::string& get_string() const{
        return buffer;
    }
};
#endif //LABA4_STRING_OUTPUT_STREAM_H