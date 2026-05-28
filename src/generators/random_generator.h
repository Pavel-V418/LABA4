#ifndef LABA4_RANDOM_GENERATOR_H
#define LABA4_RANDOM_GENERATOR_H

#include <cstdlib> // для рандома
#include "generator.h"

template<class T>
class RandomGenerator : public Generator<T> {

private:

    T min_value; // диапозон
    T max_value;

public:

    RandomGenerator(T min_value, T max_value);

    T get_next() override;

    bool has_next() const override;
};

template<class T>
RandomGenerator<T>::RandomGenerator(T min_value,T max_value)
    : min_value(min_value), max_value(max_value) {}

template<class T>
T RandomGenerator<T>::get_next() {
    return min_value + rand() % (max_value - min_value + 1);
}

template<class T>
bool RandomGenerator<T>::has_next() const {
    return true;
}

#endif //LABA4_RANDOM_GENERATOR_H

