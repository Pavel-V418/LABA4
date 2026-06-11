#include <gtest/gtest.h>
#include "../src/streams/string_input_stream.h"
#include "../src/streams/string_output_stream.h"

//STRING INPUT STREAM TESTS

// открытие и закрытие входного строкового потока
TEST(StringInputStreamTest, OpenClose) {

    StringInputStream stream("Hello");

    EXPECT_FALSE(stream.is_open());
    stream.open();
    EXPECT_TRUE(stream.is_open());

    stream.close();
    EXPECT_FALSE(stream.is_open());
}

// последовательное чтение символов из строки
TEST(StringInputStreamTest, ReadCharacters) {

    StringInputStream stream("Hello");

    stream.open();

    EXPECT_EQ(stream.read(), 'H');
    EXPECT_EQ(stream.read(), 'e');
    EXPECT_EQ(stream.read(), 'l');
    EXPECT_EQ(stream.read(), 'l');
    EXPECT_EQ(stream.read(), 'o');
}

// изменение позиции после чтения
TEST(StringInputStreamTest, PositionAfterRead) {

    StringInputStream stream("Hello");

    stream.open();

    stream.read();
    stream.read();
    stream.read();

    EXPECT_EQ(stream.get_position(), 3);
}

// переход к произвольной позиции
TEST(StringInputStreamTest, Seek) {

    StringInputStream stream("Hello");

    stream.open();
    stream.seek(2);

    EXPECT_EQ(stream.read(), 'l');
}

// проверка поддержки seek
TEST(StringInputStreamTest, CanSeek) {

    StringInputStream stream("Hello");

    EXPECT_TRUE(stream.can_seek());
}

// проверка поддержки возврата назад
TEST(StringInputStreamTest, CanGoBack) {

    StringInputStream stream("Hello");

    EXPECT_TRUE(stream.can_go_back());
}

// достижение конца потока
TEST(StringInputStreamTest, EndOfStream) {

    StringInputStream stream("ab");

    stream.open();

    EXPECT_FALSE(stream.is_end_of_stream());

    stream.read();
    stream.read();

    EXPECT_TRUE(stream.is_end_of_stream());
}

// чтение из закрытого потока
TEST(StringInputStreamTest, ReadClosedStreamThrows) {

    StringInputStream stream("Hello");

    EXPECT_THROW(stream.read(),std::logic_error);
}

// отрицательная позиция seek
TEST(StringInputStreamTest, NegativeSeekThrows) {

    StringInputStream stream("Hello");

    stream.open();

    EXPECT_THROW(stream.seek(-1),std::out_of_range);
}

// seek за пределы строки
TEST(StringInputStreamTest, SeekOutOfRangeThrows) {

    StringInputStream stream("Hello");

    stream.open();

    EXPECT_THROW(stream.seek(100),std::out_of_range);
}

//STRING OUTPUT STREAM TESTS

// открытие и закрытие выходного строкового потока
TEST(StringOutputStreamTest, OpenClose) {

    StringOutputStream stream;

    EXPECT_FALSE(stream.is_open());
    stream.open();
    EXPECT_TRUE(stream.is_open());

    stream.close();
    EXPECT_FALSE(stream.is_open());
}

// запись одного символа
TEST(StringOutputStreamTest, WriteSingleCharacter) {

    StringOutputStream stream;

    stream.open();
    stream.write('A');

    EXPECT_EQ(stream.get_string(),"A");
}

// запись нескольких символов
TEST(StringOutputStreamTest, WriteSeveralCharacters) {

    StringOutputStream stream;

    stream.open();

    stream.write('H');
    stream.write('e');
    stream.write('l');
    stream.write('l');
    stream.write('o');

    EXPECT_EQ(stream.get_string(),"Hello");
}

// изменение позиции после записи
TEST(StringOutputStreamTest, PositionAfterWrite) {

    StringOutputStream stream;

    stream.open();

    stream.write('A');
    stream.write('B');
    stream.write('C');

    EXPECT_EQ(stream.get_position(),3);
}

// очистка буфера после повторного открытия
TEST(StringOutputStreamTest, OpenClearsBuffer) {

    StringOutputStream stream;

    stream.open();

    stream.write('A');
    stream.write('B');

    stream.close();
    stream.open();

    EXPECT_EQ(stream.get_string(),"");
}

// запись в закрытый поток
TEST(StringOutputStreamTest, WriteClosedStreamThrows) {

    StringOutputStream stream;
    EXPECT_THROW(stream.write('A'),std::logic_error);
}