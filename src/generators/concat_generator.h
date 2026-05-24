#ifndef LABA4_CONCAT_GENERATOR_H
#define LABA4_CONCAT_GENERATOR_H


#include "generator.h"
#include "../../LABA_2/sequence.h"

template<class T>
class ConcatGenerator : public Generator<T> {

private:

    Sequence<T>* first;
    Sequence<T>* second;

    int current_index;

public:

    ConcatGenerator(Sequence<T>* first,Sequence<T>* second);

    T get_next() override;

    bool has_next() const override;
};

template<class T>
ConcatGenerator<T>::ConcatGenerator(Sequence<T>* first,Sequence<T>* second)
    : first(first), second(second), current_index(0) {}

template<class T>
T ConcatGenerator<T>::get_next() {
    if(current_index < first->get_length().get_value())
        return first->get(current_index++);

    return second->get(current_index++ - first->get_length().get_value());
}

template<class T>
bool ConcatGenerator<T>::has_next() const {
    return current_index < first->get_length().get_value() + second->get_length().get_value();
}


#endif //LABA4_CONCAT_GENERATOR_H
