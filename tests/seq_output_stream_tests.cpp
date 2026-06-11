#include <gtest/gtest.h>
#include "../src/streams/sequence_output_stream.h"
#include "../LABA_2/mutableArraySequence.h"

TEST(SequenceOutputStreamTest, OpenClose) {
    auto* seq = new MutableArraySequence<int>();
    SequenceOutputStream<int> stream(seq);

    EXPECT_FALSE(stream.is_open());
    stream.open();
    EXPECT_TRUE(stream.is_open());

    stream.close();
    EXPECT_FALSE(stream.is_open());

    delete seq;
}

TEST(SequenceOutputStreamTest, WriteSingleValue) {
    auto* seq = new MutableArraySequence<int>();
    SequenceOutputStream<int> stream(seq);

    stream.open();
    stream.write(10);

    EXPECT_EQ(stream.get_target()->get(0),10);

    delete seq;
}

TEST(SequenceOutputStreamTest, WriteSeveralValues) {

    auto* seq = new MutableArraySequence<int>();

    SequenceOutputStream<int> stream(seq);

    stream.open();

    stream.write(10);
    stream.write(20);
    stream.write(30);

    EXPECT_EQ(stream.get_target()->get(0),10);
    EXPECT_EQ(stream.get_target()->get(1),20);
    EXPECT_EQ(stream.get_target()->get(2),30);

    delete seq;
}

TEST(SequenceOutputStreamTest, PositionAfterWrite) {

    auto* seq = new MutableArraySequence<int>();

    SequenceOutputStream<int> stream(seq);

    stream.open();

    stream.write(10);
    stream.write(20);
    stream.write(30);

    EXPECT_EQ(stream.get_position(),3);

    delete seq;
}

TEST(SequenceOutputStreamTest, WriteClosedStreamThrows) {
    auto* seq = new MutableArraySequence<int>();
    SequenceOutputStream<int> stream(seq);

    EXPECT_THROW(stream.write(10),std::logic_error);

    delete seq;
}

TEST(SequenceOutputStreamTest, TargetLengthAfterWrite) {

    auto* seq = new MutableArraySequence<int>();

    SequenceOutputStream<int> stream(seq);

    stream.open();

    stream.write(1);
    stream.write(2);
    stream.write(3);
    stream.write(4);

    EXPECT_EQ(stream.get_target()->get_length().get_offset(),4);

    delete seq;
}

TEST(SequenceOutputStreamTest, WriteAfterCloseThrows) {

    auto* seq = new MutableArraySequence<int>();

    SequenceOutputStream<int> stream(seq);

    stream.open();
    stream.close();

    EXPECT_THROW(stream.write(123),std::logic_error);

    delete seq;
}