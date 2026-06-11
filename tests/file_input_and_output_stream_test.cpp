#include <gtest/gtest.h>
#include "../src/streams/file_input_stream.h"
#include "../src/streams/file_output_stream.h"
#include <fstream>
#include <cstdio>
#include <string>

// FILE INPUT STREAM TESTS

// Открытие и закрытие файла
TEST(FileInputStreamTest, OpenClose) {
    const std::string filename = "test_input.txt";
    {
        std::ofstream file(filename);
        file << "abc";
    }

    FileInputStream stream(filename);

    EXPECT_FALSE(stream.is_open());
    stream.open();
    EXPECT_TRUE(stream.is_open());

    stream.close();
    EXPECT_FALSE(stream.is_open());

    std::remove(filename.c_str());
}

// Чтение символов
TEST(FileInputStreamTest, ReadCharacters) {

    const std::string filename = "test_input.txt";

    {
        std::ofstream file(filename);
        file << "abc";
    }

    FileInputStream stream(filename);

    stream.open();

    EXPECT_EQ(stream.read(), 'a');
    EXPECT_EQ(stream.read(), 'b');
    EXPECT_EQ(stream.read(), 'c');

    stream.close();

    std::remove(filename.c_str());
}

// Проверка позиции
TEST(FileInputStreamTest, PositionAfterRead) {

    const std::string filename = "test_input.txt";

    {
        std::ofstream file(filename);
        file << "abc";
    }

    FileInputStream stream(filename);

    stream.open();

    stream.read();
    stream.read();

    EXPECT_EQ(stream.get_position(),2);

    stream.close();

    std::remove(filename.c_str());
}

// Проверка seek
TEST(FileInputStreamTest, Seek) {

    const std::string filename = "test_input.txt";

    {
        std::ofstream file(filename);
        file << "abcdef";
    }

    FileInputStream stream(filename);

    stream.open();
    stream.seek(3);
    EXPECT_EQ(stream.read(),'d');
    stream.close();

    std::remove(filename.c_str());
}

// Отрицательная позиция
TEST(FileInputStreamTest, NegativeSeekThrows) {

    const std::string filename = "test_input.txt";

    {
        std::ofstream file(filename);
        file << "abc";
    }

    FileInputStream stream(filename);

    stream.open();
    EXPECT_THROW(stream.seek(-1),std::out_of_range);
    stream.close();

    std::remove(filename.c_str());
}

// Чтение закрытого потока
TEST(FileInputStreamTest, ReadClosedStreamThrows) {

    const std::string filename = "test_input.txt";

    {
        std::ofstream file(filename);
        file << "abc";
    }

    FileInputStream stream(filename);

    EXPECT_THROW(stream.read(),std::logic_error);

    std::remove(filename.c_str());
}

// Несуществующий файл
TEST(FileInputStreamTest, OpenNonexistentFileThrows) {

    FileInputStream stream("file_that_does_not_exist.txt");

    EXPECT_THROW(stream.open(),std::runtime_error);
}

// FILE OUTPUT STREAM TESTS

// открытие и закрытие
TEST(FileOutputStreamTest, OpenClose) {

    const std::string filename = "test_output.txt";

    FileOutputStream stream(filename);

    EXPECT_FALSE(stream.is_open());
    stream.open();
    EXPECT_TRUE(stream.is_open());

    stream.close();
    EXPECT_FALSE(stream.is_open());

    std::remove(filename.c_str());
}

// запись 1 символа
TEST(FileOutputStreamTest, WriteSingleCharacter) {

    const std::string filename = "test_output.txt";

    FileOutputStream stream(filename);

    stream.open();
    stream.write('A');
    stream.close();

    std::ifstream file(filename);

    char c;
    file.get(c);

    EXPECT_EQ(c,'A');

    std::remove(filename.c_str());
}

// запись нескольких символов
TEST(FileOutputStreamTest, WriteSeveralCharacters) {

    const std::string filename = "test_output.txt";

    FileOutputStream stream(filename);

    stream.open();

    stream.write('H');
    stream.write('e');
    stream.write('l');
    stream.write('l');
    stream.write('o');

    stream.close();

    std::ifstream file(filename);
    std::string result;

    file >> result;

    EXPECT_EQ(result,"Hello");

    std::remove(filename.c_str());
}

// позиция после записи
TEST(FileOutputStreamTest, PositionAfterWrite) {

    const std::string filename = "test_output.txt";

    FileOutputStream stream(filename);

    stream.open();

    stream.write('A');
    stream.write('B');
    stream.write('C');

    EXPECT_EQ(stream.get_position(),3);

    stream.close();

    std::remove(filename.c_str());
}

// запись в закрытый списов
TEST(FileOutputStreamTest, WriteClosedStreamThrows) {

    const std::string filename = "test_output.txt";

    FileOutputStream stream(filename);

    EXPECT_THROW(stream.write('A'),std::logic_error);
}