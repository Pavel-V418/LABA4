#ifndef LABA4_FILE_INPUT_STREAM_H
#define LABA4_FILE_INPUT_STREAM_H

#include <fstream>
#include <string>
#include "input_stream.h"

class FileInputStream : public InputStream<char> {

private:
    std::ifstream file; // std::ifstream для чтения файлов (объект через который происходит работа с файлом)
    std::string path; // путь к файлу

    bool opened;

    int position;
    char current_char;

public:
    FileInputStream(const std::string& path)
        : path(path), opened(false), position(0), current_char('\0') {}

    ~FileInputStream() override{
        if(file.is_open())
            file.close();
    }

    void open() override{
        file.open(path);

        if (!file.is_open())
            throw std::runtime_error("Failed to open file");

        opened = true;
        position = 0;
    }

    void close() override{
        file.close();

        opened = false;
    }

    bool is_open() const override {
        return opened;
    }

    bool is_end_of_stream() const override {
        return file.eof(); //проверяет дошел ли поток до конца файла
    }

    bool can_seek() const override {
        return true;
    }

    bool can_go_back() const override {
        return true;
    }

    int get_position() const override {
        return position;
    }

    void seek(int pos) override {
        if (pos < 0)
            throw std::out_of_range("Negative seek position");

        file.clear(); // сбрасывает ошибки потока
        file.seekg(pos); // перемещение указателя чтения

        if (file.fail())
            throw std::runtime_error("seek failed");

        position = pos;
    }

    const char& read() override {
        if(!opened)
            throw std::logic_error("Stream is closed");

        if(!file.get(current_char))
            throw std::out_of_range("End of stream reached");

        position++;

        return current_char;
    }

};

#endif //LABA4_FILE_INPUT_STREAM_H
