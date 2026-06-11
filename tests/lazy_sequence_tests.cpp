#include <gtest/gtest.h>
#include "../src/lazy/lazy_sequence.h"
#include "../src/generators/stream_number_generator.h"

static LazySequence<int>* create_sequence() {

    auto factory = []() {
        return new StreamNumberGenerator<int>(1, 1);
    };

    return new LazySequence<int>(factory,Cardinal::infinity(),10);
}

// Проверка получения элементов по индексу
TEST(LazySequenceTest, GetElements) {

    auto* seq = create_sequence();

    EXPECT_EQ(seq->get(0), 1);
    EXPECT_EQ(seq->get(1), 2);
    EXPECT_EQ(seq->get(5), 6);
    EXPECT_EQ(seq->get(10), 11);

    delete seq;
}

// Проверка отрицательного индекса
TEST(LazySequenceTest, NegativeIndexThrows) {
    auto* seq = create_sequence();

    EXPECT_THROW(seq->get(-1),std::out_of_range);

    delete seq;
}

// Проверка первого элемента.
TEST(LazySequenceTest, GetFirst) {

    auto* seq = create_sequence();

    EXPECT_EQ(seq->get_first(),1);

    delete seq;
}

// Проверка количества материализованных элементов
TEST(LazySequenceTest, MaterializedCount) {

    auto* seq = create_sequence();

    EXPECT_EQ(seq->get_materialized_count(),0);

    seq->get(5);

    EXPECT_EQ(seq->get_materialized_count(),6);

    delete seq;
}

// Проверка take()
TEST(LazySequenceTest, Take) {

    auto* seq = create_sequence();

    auto* taken = seq->take(5);

    EXPECT_EQ(
        taken->get_length().get_offset(),
        5
    );

    EXPECT_EQ(taken->get(0), 1);
    EXPECT_EQ(taken->get(1), 2);
    EXPECT_EQ(taken->get(2), 3);
    EXPECT_EQ(taken->get(3), 4);
    EXPECT_EQ(taken->get(4), 5);

    delete taken;
    delete seq;
}

// Проверка skip()
TEST(LazySequenceTest, Skip) {

    auto* seq = create_sequence();

    auto* skipped = seq->skip(5);

    EXPECT_EQ(skipped->get(0), 6);
    EXPECT_EQ(skipped->get(1), 7);
    EXPECT_EQ(skipped->get(2), 8);
    EXPECT_EQ(skipped->get(3), 9);
    EXPECT_EQ(skipped->get(4), 10);

    delete skipped;
    delete seq;
}

// Проверка map()
TEST(LazySequenceTest, Map) {

    auto* seq = create_sequence();

    auto* mapped = seq->map(
            [](const int& x) {
                return x * 2;
            }
        );

    EXPECT_EQ(mapped->get(0), 2);
    EXPECT_EQ(mapped->get(1), 4);
    EXPECT_EQ(mapped->get(2), 6);
    EXPECT_EQ(mapped->get(3), 8);
    EXPECT_EQ(mapped->get(4), 10);

    delete mapped;
    delete seq;
}

// Проверка фильтрации чётных чисел
TEST(LazySequenceTest, FilterEvenNumbers) {

    auto* seq = create_sequence();

    auto* filtered = seq->where(
            [](const int& x) {
                return x % 2 == 0;
            }
        );

    EXPECT_EQ(filtered->get(0), 2);
    EXPECT_EQ(filtered->get(1), 4);
    EXPECT_EQ(filtered->get(2), 6);
    EXPECT_EQ(filtered->get(3), 8);
    EXPECT_EQ(filtered->get(4), 10);

    delete filtered;
    delete seq;
}

// Проверка подпоследовательности
TEST(LazySequenceTest, Subsequence) {

    auto* seq = create_sequence();

    auto* sub = seq->get_subsequence(3, 7);

    EXPECT_EQ(sub->get(0), 4);
    EXPECT_EQ(sub->get(1), 5);
    EXPECT_EQ(sub->get(2), 6);
    EXPECT_EQ(sub->get(3), 7);
    EXPECT_EQ(sub->get(4), 8);

    delete sub;
    delete seq;
}

// Проверка некорректного диапазона подпоследовательности
TEST(LazySequenceTest, InvalidSubsequenceThrows) {

    auto* seq = create_sequence();

    EXPECT_THROW(seq->get_subsequence(10, 5),std::out_of_range);

    delete seq;
}

// Проверка отрицательного параметра take()
TEST(LazySequenceTest, NegativeTakeThrows) {

    auto* seq = create_sequence();

    EXPECT_THROW(seq->take(-1),std::out_of_range);

    delete seq;
}

// Проверка отрицательного параметра skip()
TEST(LazySequenceTest, NegativeSkipThrows) {

    auto* seq = create_sequence();

    EXPECT_THROW(seq->skip(-1),std::out_of_range);

    delete seq;
}

// Проверка append для бесконечной последовательности
TEST(LazySequenceTest, AppendInfiniteSequence) {

    auto* seq = create_sequence();
    auto* appended = seq->append(100);

    EXPECT_EQ(appended->get(Cardinal(1,1)),100);

    delete appended;
    delete seq;
}

// Проверка цепочки append
TEST(LazySequenceTest, MultipleAppendInfiniteSequence) {

    auto* seq = create_sequence();

    auto* s1 = seq->append(100);
    auto* s2 = s1->append(200);
    auto* s3 = s2->append(300);

    EXPECT_EQ(s3->get(Cardinal(1,1)), 100);
    EXPECT_EQ(s3->get(Cardinal(1,2)), 200);
    EXPECT_EQ(s3->get(Cardinal(1,3)), 300);

    delete s3;
    delete s2;
    delete s1;
    delete seq;
}

// Проверка prepend
TEST(LazySequenceTest, Prepend) {

    auto* seq = create_sequence();

    auto* prepended = seq->prepend(999);

    EXPECT_EQ(prepended->get(0), 999);
    EXPECT_EQ(prepended->get(1), 1);
    EXPECT_EQ(prepended->get(2), 2);

    delete prepended;
    delete seq;
}

// Проверка insert_at в начало
TEST(LazySequenceTest, InsertAtBeginning) {

    auto* seq = create_sequence();

    auto* inserted = seq->insert_at(999,0);

    EXPECT_EQ(inserted->get(0),999);
    EXPECT_EQ(inserted->get(1),1);
    EXPECT_EQ(inserted->get(2),2);

    delete inserted;
    delete seq;
}

// Проверка insert_at в середину
TEST(LazySequenceTest, InsertAtMiddle) {

    auto* seq = create_sequence();

    auto* inserted = seq->insert_at(999,3);

    EXPECT_EQ(inserted->get(0),1);
    EXPECT_EQ(inserted->get(1),2);
    EXPECT_EQ(inserted->get(2),3);

    EXPECT_EQ(inserted->get(3),999);

    EXPECT_EQ(inserted->get(4),4);
    EXPECT_EQ(inserted->get(5),5);

    delete inserted;
    delete seq;
}

// Проверка некорректного индекса вставки
TEST(LazySequenceTest, InsertAtNegativeIndexThrows) {

    auto* seq = create_sequence();

    EXPECT_THROW(seq->insert_at(5,-1),std::out_of_range);

    delete seq;
}

// Проверка concat двух бесконечных последовательностей
TEST(LazySequenceTest, ConcatInfiniteSequences) {

    auto factory = []() {
        return new StreamNumberGenerator<int>(100,1);
    };

    auto* seq1 = create_sequence();

    auto* seq2 =new LazySequence<int>(factory,Cardinal::infinity(),10);

    auto* concat = seq1->concat(seq2);

    EXPECT_EQ(concat->get_length().get_omega_count(),2);

    EXPECT_EQ(concat->get(Cardinal(0,5)),6);

    EXPECT_EQ(concat->get(Cardinal(1,5)),105);

    delete concat;
    delete seq2;
    delete seq1;
}

// Проверка длины после append
TEST(LazySequenceTest, LengthAfterAppend) {

    auto* seq = create_sequence();

    auto* appended = seq->append(100);

    EXPECT_EQ(appended->get_length().get_omega_count(),1);

    EXPECT_EQ(appended->get_length().get_offset(), 1);

    delete appended;
    delete seq;
}

// Проверка длины после concat
TEST(LazySequenceTest, LengthAfterConcat) {

    auto* seq1 = create_sequence();
    auto* seq2 = create_sequence();

    auto* concat = seq1->concat(seq2);

    EXPECT_EQ(concat->get_length().get_omega_count(),2);

    EXPECT_EQ(concat->get_length().get_offset(),0);

    delete concat;
    delete seq2;
    delete seq1;
}

// Проверка zip
TEST(LazySequenceTest, ZipSequences) {

    auto* seq1 = create_sequence();

    auto factory = []() {
        return new StreamNumberGenerator<int>(10,10);
    };

    auto* seq2 = new LazySequence<int>(factory,Cardinal::infinity(),10);

    auto* zipped = seq1->zip(seq2);

    EXPECT_EQ(zipped->get(0).first,1);
    EXPECT_EQ(zipped->get(0).second,10);
    EXPECT_EQ(zipped->get(1).first,2);
    EXPECT_EQ(zipped->get(1).second,20);

    delete zipped;
    delete seq2;
    delete seq1;
}