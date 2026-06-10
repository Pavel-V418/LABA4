#ifndef LABA4_STREAM_NUMBER_GENERATOR_H
#define LABA4_STREAM_NUMBER_GENERATOR_H

#include "generator.h"
// моделирует арифметическую прогрессию
template<class T>
class StreamNumberGenerator : public Generator<T> {

private:

    T start;
    T current;
    T step;

public:

    StreamNumberGenerator(T start = 0, T step = 1);

    T get_next() override;
    T get(const Cardinal& index) override;

    bool has_next() const override;
};

template<class T>
StreamNumberGenerator<T>::StreamNumberGenerator(T start, T step)
    : start(start), current(start), step(step) {}

template<class T>
T StreamNumberGenerator<T>::get_next() {

    T value = current;

    current += step;

    return value;
}

template<class T>
T StreamNumberGenerator<T>::get(const Cardinal& index) {
    if(index.get_omega_count() != 0)
        throw std::logic_error("Arithmetic generator does not support omega indices");

    return start + step * index.get_offset();
}

template<class T>
bool StreamNumberGenerator<T>::has_next() const {
    return true;
}

#endif //LABA4_STREAM_NUMBER_GENERATOR_H