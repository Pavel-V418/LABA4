#ifndef LABA4_SLIDING_CACHE_H
#define LABA4_SLIDING_CACHE_H

#include "../../LABA_2/dynamicArray.h"
#include <stdexcept>

template<class T>
class SlidingCache {

private:
    DynamicArray<T> buffer; // память окна

    int capacity; // max размер окна
    int count;

    int first_physical_index; // где физически в буфере лежит начало окна
    int first_logical_index;
    int last_logical_index;

public:

    SlidingCache(int capacity = 100);

    bool is_empty() const;

    int get_count() const;
    int get_capacity() const;
    int get_first_index() const;
    int get_last_index() const;

    bool contains(int logical_index) const;

    const T& get(int logical_index) const;

    void push(const T& item, int logical_index);

    void clear();
};

template<class T>
SlidingCache<T>::SlidingCache(int capacity)
    : buffer(capacity), capacity(capacity), count(0), first_physical_index(0),
    first_logical_index(0), last_logical_index(0){
    if(capacity < 1)
        throw std::invalid_argument("SlidingCache capacity must be >= 1");
}

template<class T>
bool SlidingCache<T>::is_empty() const {
    return count == 0;
}

template<class T>
int SlidingCache<T>::get_count() const {
    return count;
}

template<class T>
int SlidingCache<T>::get_capacity() const {
    return capacity;
}

template<class T>
int SlidingCache<T>::get_first_index() const {
    if(count == 0)
        throw std::logic_error("Cache is empty");

    return first_logical_index;
}

template<class T>
int SlidingCache<T>::get_last_index() const {
    if(count == 0)
        throw std::logic_error("Cache is empty");

    return last_logical_index;
}

template<class T>
bool SlidingCache<T>::contains(int logical_index) const { // находится ли элемент внутри окна
    if(count == 0)
        return false;

    return logical_index >= first_logical_index && logical_index <= last_logical_index;
}

template<class T>
const T& SlidingCache<T>::get(int logical_index) const {
    if(!contains(logical_index))
        throw std::out_of_range("Index is outside sliding window");

    int offset = logical_index - first_logical_index;

    int physical_index =(first_physical_index + static_cast<int>(offset)) %capacity;

    return buffer.get(physical_index);
}

template<class T>
void SlidingCache<T>::push(const T& item, int logical_index) {

    if(count == 0){
        buffer.set(item,0);

        first_physical_index = 0;
        first_logical_index = logical_index;
        last_logical_index = logical_index;

        count = 1;

        return;
    }

    if(count < capacity){
        int new_physical_index =
            (first_physical_index + count) % capacity;

        buffer.set(item, new_physical_index);

        last_logical_index = logical_index;

        count++;

        return;
    }

    //Перезаписываем самый старый элемент
    buffer.set(item,first_physical_index);

    first_physical_index =(first_physical_index + 1) % capacity;
    first_logical_index =logical_index - capacity + 1;
    last_logical_index = logical_index;
}

template<class T>
void SlidingCache<T>::clear() {

    first_physical_index = 0;
    first_logical_index = 0;
    last_logical_index = 0;
    count = 0;
}

#endif //LABA4_SLIDING_CACHE_H