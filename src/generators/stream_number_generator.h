#ifndef LABA4_STREAM_NUMBER_GENERATOR_H
#define LABA4_STREAM_NUMBER_GENERATOR_H

#include "generator.h"
// моделирует бесконечную арифметическую прогрессию
template<class T>
class StreamNumberGenerator : public Generator<T> {

private:

    T current;
    T step; // на сколько сдвинуться дальше

public:

    StreamNumberGenerator(T start = 0, T step = 1);

    T get_next() override;

    bool has_next() const override;
};

template<class T>
StreamNumberGenerator<T>::StreamNumberGenerator(T start, T step)
    : current(start), step(step) {}

template<class T>
T StreamNumberGenerator<T>::get_next() {

    T value = current;

    current += step;

    return value;
}

template<class T>
bool StreamNumberGenerator<T>::has_next() const {
    return true;
}

#endif //LABA4_STREAM_NUMBER_GENERATOR_H