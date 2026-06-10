#ifndef LABA4_FILTER_GENERATOR_H
#define LABA4_FILTER_GENERATOR_H

#include "generator.h"

template<class T>
class FilterGenerator : public Generator<T> {

public:
    FilterGenerator(Sequence<T>* source,std::function<bool(const T&)> predicate);

    T get_next() override;
    T get(const Cardinal& index) override;

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

    return current_index < source->get_length().get_offset();;
}

template<class T>
T FilterGenerator<T>::get(const Cardinal& index) {

    if(index.get_omega_count() != 0)
        throw std::logic_error("FilterGenerator does not support omega indices");

    int target = index.get_offset();

    int found = -1;
    int source_index = 0;

    while(true) {
        T value = source->get(source_index++);

        if(predicate(value)) {
            found++;

            if(found == target)
                return value;
        }
    }
}
#endif //LABA4_FILTER_GENERATOR_H
