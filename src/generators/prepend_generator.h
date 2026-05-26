#ifndef LABA4_PREPEND_GENERATOR_H
#define LABA4_PREPEND_GENERATOR_H

#include "generator.h"

template<class T>
class PrependGenerator : public Generator<T> {

private:
    Sequence<T>* source;

    T prepended_item;

    int current_index;

public:
    PrependGenerator(Sequence<T>* source,const T& item);

    T get_next() override;

    bool has_next() const override;
};


template<class T>
PrependGenerator<T>::PrependGenerator(Sequence<T>* source, const T& item)
    : source(source), prepended_item(item), current_index(0) {}

template<class T>
T PrependGenerator<T>::get_next() {
    if(current_index == 0){
        current_index++;

        return prepended_item;
    }

    return source->get(
        current_index++ - 1
    );
}

template<class T>
bool PrependGenerator<T>::has_next() const {
    Cardinal length = source->get_length();

    if(length.is_infinite())
        return true;

    return current_index <= length.get_value();
}

#endif //LABA4_PREPEND_GENERATOR_H