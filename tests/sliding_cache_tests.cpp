#include <gtest/gtest.h>
#include "../src/lazy/sliding_cache.h"

// Проверка создания пустого кэша
TEST(SlidingCacheTest, DefaultConstruction) {

    SlidingCache<int> cache(5);

    EXPECT_TRUE(cache.is_empty());
    EXPECT_EQ(cache.get_count(), 0);
    EXPECT_EQ(cache.get_capacity(), 5);
}

// Проверка некорректного размера окна
TEST(SlidingCacheTest, InvalidCapacityThrows) {
    EXPECT_THROW(SlidingCache<int> cache(0),std::invalid_argument);
}

// Проверка добавления первого элемента
TEST(SlidingCacheTest, PushSingleElement) {

    SlidingCache<int> cache(5);

    cache.push(10, 0);

    EXPECT_FALSE(cache.is_empty());
    EXPECT_EQ(cache.get_count(), 1);

    EXPECT_TRUE(cache.contains(0));
    EXPECT_EQ(cache.get(0), 10);

    EXPECT_EQ(cache.get_first_index(), 0);
    EXPECT_EQ(cache.get_last_index(), 0);
}

// Проверка добавления нескольких элементов
TEST(SlidingCacheTest, PushSeveralElements) {

    SlidingCache<int> cache(5);

    cache.push(10, 0);
    cache.push(20, 1);
    cache.push(30, 2);

    EXPECT_EQ(cache.get_count(), 3);

    EXPECT_EQ(cache.get(0), 10);
    EXPECT_EQ(cache.get(1), 20);
    EXPECT_EQ(cache.get(2), 30);
}

// Проверка contains()
TEST(SlidingCacheTest, ContainsWorksCorrectly) {

    SlidingCache<int> cache(5);

    cache.push(10, 0);
    cache.push(20, 1);
    cache.push(30, 2);

    EXPECT_TRUE(cache.contains(0));
    EXPECT_TRUE(cache.contains(1));
    EXPECT_TRUE(cache.contains(2));

    EXPECT_FALSE(cache.contains(3));
    EXPECT_FALSE(cache.contains(-1));
}

// Проверка получения индексов окна
TEST(SlidingCacheTest, FirstAndLastIndices) {

    SlidingCache<int> cache(5);

    cache.push(10, 5);
    cache.push(20, 6);
    cache.push(30, 7);

    EXPECT_EQ(cache.get_first_index(), 5);
    EXPECT_EQ(cache.get_last_index(), 7);
}

// Проверка переполнения окна
TEST(SlidingCacheTest, SlidingWindowOverwrite) {

    SlidingCache<int> cache(3);

    cache.push(10, 0);
    cache.push(20, 1);
    cache.push(30, 2);

    cache.push(40, 3);

    EXPECT_EQ(cache.get_count(), 3);

    EXPECT_FALSE(cache.contains(0));

    EXPECT_TRUE(cache.contains(1));
    EXPECT_TRUE(cache.contains(2));
    EXPECT_TRUE(cache.contains(3));

    EXPECT_EQ(cache.get(1), 20);
    EXPECT_EQ(cache.get(2), 30);
    EXPECT_EQ(cache.get(3), 40);

    EXPECT_EQ(cache.get_first_index(), 1);
    EXPECT_EQ(cache.get_last_index(), 3);
}

// Проверка нескольких переполнений подряд
TEST(SlidingCacheTest, MultipleOverwrites) {

    SlidingCache<int> cache(3);

    cache.push(10, 0);
    cache.push(20, 1);
    cache.push(30, 2);
    cache.push(40, 3);
    cache.push(50, 4);

    EXPECT_FALSE(cache.contains(0));
    EXPECT_FALSE(cache.contains(1));

    EXPECT_TRUE(cache.contains(2));
    EXPECT_TRUE(cache.contains(3));
    EXPECT_TRUE(cache.contains(4));

    EXPECT_EQ(cache.get(2), 30);
    EXPECT_EQ(cache.get(3), 40);
    EXPECT_EQ(cache.get(4), 50);
}

// Проверка очистки кэша
TEST(SlidingCacheTest, ClearCache) {

    SlidingCache<int> cache(5);

    cache.push(10, 0);
    cache.push(20, 1);

    cache.clear();

    EXPECT_TRUE(cache.is_empty());
    EXPECT_EQ(cache.get_count(), 0);

    EXPECT_FALSE(cache.contains(0));
    EXPECT_FALSE(cache.contains(1));
}

// Проверка исключения при запросе отсутствующего элемента
TEST(SlidingCacheTest, GetOutsideWindowThrows) {

    SlidingCache<int> cache(3);

    cache.push(10, 0);
    cache.push(20, 1);

    EXPECT_THROW(cache.get(5),std::out_of_range);
}

// Проверка исключения при запросе индексов пустого окна
TEST(SlidingCacheTest, EmptyCacheIndicesThrow) {

    SlidingCache<int> cache(5);

    EXPECT_THROW(cache.get_first_index(),std::logic_error);
    EXPECT_THROW(cache.get_last_index(),std::logic_error);
}