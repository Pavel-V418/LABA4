#ifndef LABA4_FILTER_GENERATOR_H
#define LABA4_FILTER_GENERATOR_H

#include "generator.h"

template<class T>
class FilterGenerator : public Generator<T> {

public:
    FilterGenerator(Sequence<T>* source,std::function<bool(const T&)> predicate);

    T get_next() override;

    bool has_next() const override;

private:
    Sequence<T>* source;

    std::function<bool(const T&)> predicate;

    int current_index;

};

template<class T>
FilterGenerator<T>::FilterGenerator(Sequence<T> *source, std::function<bool(const T &)> predicate)
    : source(source), predicate(predicate), current_index(0) {}

template<class T>
T FilterGenerator<T>::get_next() {
    while(true){
        T value = source->get(current_index++);

        if(predicate(value))
            return value;
    }
}

template<class T>
bool FilterGenerator<T>::has_next() const {
    if(source->get_length().is_infinite())
        return true;

    return current_index < source->get_length().get_value();
}

#endif //LABA4_FILTER_GENERATOR_H
