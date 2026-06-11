#include <gtest/gtest.h>
#include "../src/lazy/cardinal.h"

// тесты на конструкторы
TEST(CardinalTest, DefaultConstructor) {

    Cardinal c;

    EXPECT_EQ(c.get_omega_count(), 0);
    EXPECT_EQ(c.get_offset(), 0);
}

TEST(CardinalTest, FiniteConstructor) {

    Cardinal c(10);

    EXPECT_EQ(c.get_omega_count(), 0);
    EXPECT_EQ(c.get_offset(), 10);
}

TEST(CardinalTest, InfiniteConstructor) {

    Cardinal c(2, 5);

    EXPECT_EQ(c.get_omega_count(), 2);
    EXPECT_EQ(c.get_offset(), 5);
}

// проверка infinity()
TEST(CardinalTest, InfinityFactory) {

    Cardinal inf = Cardinal::infinity();

    EXPECT_TRUE(inf.is_infinite());

    EXPECT_EQ(inf.get_omega_count(), 1);
    EXPECT_EQ(inf.get_offset(), 0);
}

// Проверка операторов
TEST(CardinalTest, Equality) {

    Cardinal a(1, 5);
    Cardinal b(1, 5);

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

TEST(CardinalTest, Inequality) {

    Cardinal a(1, 5);
    Cardinal b(1, 6);

    EXPECT_TRUE(a != b);
    EXPECT_FALSE(a == b);
}

TEST(CardinalTest, AdditionFinite) {

    Cardinal a(10);
    Cardinal b(5);

    Cardinal result = a + b;

    EXPECT_EQ(result.get_omega_count(), 0);
    EXPECT_EQ(result.get_offset(), 15);
}

TEST(CardinalTest, AdditionInfinite) {

    Cardinal a(1, 5);
    Cardinal b(2, 3);

    Cardinal result = a + b;

    EXPECT_EQ(result.get_omega_count(), 3);
    EXPECT_EQ(result.get_offset(), 8);
}

TEST(CardinalTest, SubtractionFinite) {

    Cardinal a(10);
    Cardinal b(3);

    Cardinal result = a - b;

    EXPECT_EQ(result.get_omega_count(), 0);
    EXPECT_EQ(result.get_offset(), 7);
}

TEST(CardinalTest, SubtractionInfinite) {

    Cardinal a(3, 10);
    Cardinal b(1, 4);

    Cardinal result = a - b;

    EXPECT_EQ(result.get_omega_count(), 2);
    EXPECT_EQ(result.get_offset(), 6);
}

// Негативные тесты
TEST(CardinalTest, SubtractionNegativeOmegaThrows) {

    Cardinal a(1, 5);
    Cardinal b(2, 1);

    EXPECT_THROW(a - b,std::logic_error);
}

TEST(CardinalTest, SubtractionNegativeOffsetThrows) {

    Cardinal a(1, 5);
    Cardinal b(1, 10);

    EXPECT_THROW(a - b,std::logic_error);
}