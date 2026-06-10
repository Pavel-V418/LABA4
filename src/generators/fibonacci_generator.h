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
    T get(const Cardinal& index) override;

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
T FibonacciGenerator<T>::get(const Cardinal& index) {
    if(index.get_omega_count() != 0)
        throw std::logic_error("Fibonacci generator does not support omega indices");

    int n = index.get_offset();

    if(n == 0)
        return 1;
    
    if(n == 1)
        return 1;

    T a = 1;
    T b = 1;

    for(int i = 2; i <= n; i++) {
        T c = a + b;

        a = b;
        b = c;
    }

    return b;
}

template<class T>
bool FibonacciGenerator<T>::has_next() const {
    return true;
}

#endif //LABA4_FIBONACCI_GENERATOR_H
