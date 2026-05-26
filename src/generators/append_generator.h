#ifndef LABA4_APPEND_GENERATOR_H
#define LABA4_APPEND_GENERATOR_H

#include "generator.h"

template<class T>
class AppendGenerator : public Generator<T> {

private:
    Sequence<T>* source;

    T appended_item;

    int current_index;

public:

    AppendGenerator(Sequence<T>* source,const T& item);

    T get_next() override;

    bool has_next() const override;
};

template<class T>
AppendGenerator<T>::AppendGenerator(Sequence<T>* source,const T& item)
    : source(source), appended_item(item), current_index(0) {}

template<class T>
T AppendGenerator<T>::get_next() {
    Cardinal length = source->get_length();

    if(current_index < length.get_value())
        return source->get(current_index++);

    current_index++;

    return appended_item;
}

template<class T>
bool AppendGenerator<T>::has_next() const {
    Cardinal length = source->get_length();

    if(length.is_infinite())
        return true;

    return current_index <= length.get_value();
}

#endif //LABA4_APPEND_GENERATOR_H