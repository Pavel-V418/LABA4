#ifndef LABA4_FILE_OUTPUT_STREAM_H
#define LABA4_FILE_OUTPUT_STREAM_H

#include <fstream>
#include <string>

#include "output_stream.h"

class FileOutputStream
    : public OutputStream<char> {

private:

    std::ofstream file; // объект для записи в файл
    std::string path;

    bool opened;

    int position;

public:

    FileOutputStream(const std::string& path)
        : path(path), opened(false), position(0) {}

    ~FileOutputStream() override {
        if (file.is_open())
            file.close();
    }

    void open() override {
        file.open(path);

        if (!file.is_open())
            throw std::runtime_error("Failed to open file");

        opened = true;

        position = 0;
    }

    void close() override {
        file.close();

        opened = false;
    }

    bool is_open() const override {
        return opened;
    }

    int get_position() const override {
        return position;
    }

    void write(const char& item) override {
        if (!is_open())
            throw std::logic_error("Stream is closed");

        file.put(item);

        if (file.fail())
            throw std::runtime_error("Write failed");

        position++;
    }
};

#endif //LABA4_FILE_OUTPUT_STREAM_H
