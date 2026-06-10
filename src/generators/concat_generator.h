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
    T get(const Cardinal& index) override;

    bool has_next() const override;
};

template<class T>
ConcatGenerator<T>::ConcatGenerator(Sequence<T>* first,Sequence<T>* second)
    : first(first), second(second), current_index(0) {}

template<class T>
T ConcatGenerator<T>::get_next() {

    Cardinal first_length = first->get_length();

    if(first_length.is_infinite())
        return first->get(current_index++);

    if(current_index < first_length.get_offset())
        return first->get(current_index++);

    return second->get(
        current_index++ - first_length.get_offset()
    );
}

template<class T>
T ConcatGenerator<T>::get(const Cardinal& index) {

    Cardinal first_length = first->get_length();

    if(index.get_omega_count() < first_length.get_omega_count())
        return first->get(index);

    return second->get(Cardinal(index.get_omega_count() - first_length.get_omega_count(),index.get_offset()));
}

template<class T>
bool ConcatGenerator<T>::has_next() const {

    Cardinal first_length = first->get_length();
    Cardinal second_length = second->get_length();

    if(first_length.is_infinite() || second_length.is_infinite())
        return true;

    return current_index <first_length.get_offset() + second_length.get_offset();
}


#endif //LABA4_CONCAT_GENERATOR_H
