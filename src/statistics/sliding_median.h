#ifndef LABA4_SLIDING_MEDIAN_H
#define LABA4_SLIDING_MEDIAN_H

#include "../lazy/sliding_cache.h"
#include <algorithm>
#include <stdexcept>

template<class T>
class SlidingMedian {

private:
    SlidingCache<T> window;

    void bubble_sort(DynamicArray<T>& array, int size) const;

public:

    SlidingMedian(int window_size);

    void add(const T& value);

    double get_median() const;

    int get_window_size() const;

    bool is_empty() const;
};

template<class T>
SlidingMedian<T>::SlidingMedian(int window_size)
    : window(window_size) {}

template<class T>
void SlidingMedian<T>::add(const T& value) {
    int logical_index;

    if(window.is_empty())
        logical_index = 0;

    else
        logical_index = window.get_last_index() + 1;

    window.push(value,logical_index);
}

template<class T>
void SlidingMedian<T>::bubble_sort(DynamicArray<T>& array,int size) const {
    for(int i = 0; i < size - 1; i++){

        for(int j = 0; j < size - i - 1; j++){

            if(array.get(j) > array.get(j + 1)){
                T temp = array.get(j);

                array.set(array.get(j + 1),j);

                array.set(temp,j + 1);
            }
        } // j
    }// i
}

template<class T>
double SlidingMedian<T>::get_median() const {
    if(window.is_empty())
        throw std::logic_error("Median of empty window");

    int size = window.get_count();

    DynamicArray<T> values(size);

    int logical_index = window.get_first_index();

    for(int i = 0; i < size; i++)
        values.set(window.get(logical_index + i), i);

    bubble_sort(values, size);

    if(size % 2 == 1)
        return static_cast<double>(values.get(size / 2));

    return(static_cast<double>(values.get(size / 2 - 1)) + static_cast<double>(values.get(size / 2))) / 2.0;
}

template<class T>
int SlidingMedian<T>::get_window_size() const {
    return window.get_capacity();
}

template<class T>
bool SlidingMedian<T>::is_empty() const {
    return window.is_empty();
}

#endif //LABA4_SLIDING_MEDIAN_H