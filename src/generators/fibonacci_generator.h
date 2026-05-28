#ifndef LABA4_FIBONACCI_GENERATOR_H
#define LABA4_FIBONACCI_GENERATOR_H

#include "generator.h"

template<class T>
class FibonacciGenerator : public Generator<T> {

private:

    T previous;
    T current;

public:

    FibonacciGenerator();

    T get_next() override;

    bool has_next() const override;
};

template<class T>
FibonacciGenerator<T>::FibonacciGenerator()
    : previous(0), current(1) {}

template<class T>
T FibonacciGenerator<T>::get_next() {

    T value = current;
    T next = previous + current;

    previous = current;
    current = next;

    return value;
}

template<class T>
bool FibonacciGenerator<T>::has_next() const {
    return true;
}

#endif //LABA4_FIBONACCI_GENERATOR_H
