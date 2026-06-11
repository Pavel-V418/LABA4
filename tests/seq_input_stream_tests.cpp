#include <gtest/gtest.h>
#include "../src/streams/sequence_input_stream.h"
#include "../src/lazy/lazy_sequence.h"
#include "../src/generators/stream_number_generator.h"

static LazySequence<int>* create_stream_sequence() {
    auto factory = []() {
        return new StreamNumberGenerator<int>(1,1);
    };

    return new LazySequence<int>(factory,Cardinal::infinity(),10);
}

TEST(SequenceInputStreamTest, OpenClose) {
    auto* seq = create_stream_sequence();

    SequenceInputStream<int> stream(seq);

    EXPECT_FALSE(stream.is_open());
    stream.open();
    EXPECT_TRUE(stream.is_open());

    stream.close();
    EXPECT_FALSE(stream.is_open());

    delete seq;
}

TEST(SequenceInputStreamTest, ReadValues) {
    auto* seq = create_stream_sequence();

    SequenceInputStream<int> stream(seq);

    stream.open();

    EXPECT_EQ(stream.read(), 1);
    EXPECT_EQ(stream.read(), 2);
    EXPECT_EQ(stream.read(), 3);
    EXPECT_EQ(stream.read(), 4);
    EXPECT_EQ(stream.read(), 5);

    delete seq;
}

TEST(SequenceInputStreamTest, PositionAfterRead) {

    auto* seq = create_stream_sequence();

    SequenceInputStream<int> stream(seq);

    stream.open();

    stream.read();
    stream.read();
    stream.read();

    EXPECT_EQ(stream.get_position(),3);

    delete seq;
}

TEST(SequenceInputStreamTest, Seek) {

    auto* seq = create_stream_sequence();

    SequenceInputStream<int> stream(seq);

    stream.open();
    stream.seek(10);

    EXPECT_EQ(stream.get_position(),10);
    EXPECT_EQ(stream.read(),11);

    delete seq;
}

TEST(SequenceInputStreamTest, CanSeek) {

    auto* seq = create_stream_sequence();
    SequenceInputStream<int> stream(seq);

    EXPECT_TRUE(stream.can_seek());

    delete seq;
}

TEST(SequenceInputStreamTest, CanGoBack) {

    auto* seq = create_stream_sequence();
    SequenceInputStream<int> stream(seq);

    EXPECT_TRUE(stream.can_go_back());

    delete seq;
}

TEST(SequenceInputStreamTest, InfiniteSequenceNeverEnds) {
    auto* seq = create_stream_sequence();
    SequenceInputStream<int> stream(seq);

    stream.open();

    EXPECT_FALSE(stream.is_end_of_stream());
    stream.seek(1000);
    EXPECT_FALSE(stream.is_end_of_stream());

    delete seq;
}

TEST(SequenceInputStreamTest, ReadClosedStreamThrows) {
    auto* seq = create_stream_sequence();

    SequenceInputStream<int> stream(seq);
    EXPECT_THROW(stream.read(),std::logic_error);

    delete seq;
}

TEST(SequenceInputStreamTest, NegativeSeekThrows) {
    auto* seq = create_stream_sequence();
    SequenceInputStream<int> stream(seq);

    stream.open();

    EXPECT_THROW(stream.seek(-1),std::out_of_range);

    delete seq;
}