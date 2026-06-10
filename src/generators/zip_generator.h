#ifndef LABA4_ZIP_GENERATOR_H
#define LABA4_ZIP_GENERATOR_H

#include "generator.h"
#include "../utils/pair.h"

template<class T1, class T2> // возможность работать с двумя разными типами
class ZipGenerator : public Generator<Pair<T1,T2>> {

private:
    Sequence<T1>* first;
    Sequence<T2>* second;

    int current_index;

public:

    ZipGenerator(Sequence<T1>* first, Sequence<T2>* second);

    Pair<T1,T2> get_next() override;
    Pair<T1,T2> get(const Cardinal& index) override;

    bool has_next() const override;
};

template<class T1, class T2>
ZipGenerator<T1,T2>::ZipGenerator(Sequence<T1>* first, Sequence<T2>* second)
    : first(first), second(second), current_index(0) {}

template<class T1, class T2>
Pair<T1,T2> ZipGenerator<T1,T2>::get_next() {
    Pair<T1,T2> result(first->get(current_index),second->get(current_index));

    current_index++;

    return result;
}

template<class T1, class T2>
bool ZipGenerator<T1,T2>::has_next() const {
    Cardinal first_length = first->get_length();
    Cardinal second_length = second->get_length();

    if(first_length.is_infinite() && second_length.is_infinite())
        return true;

    if(!first_length.is_infinite()){
        if(current_index >= first_length.get_offset())
            return false;
    }

    if(!second_length.is_infinite()){
        if(current_index >= second_length.get_offset())
            return false;
    }

    return true;
}

template<class T1, class T2>
Pair<T1,T2> ZipGenerator<T1,T2>::get(const Cardinal& index) {
    return Pair<T1,T2>(first->get(index),second->get(index));
}
#endif //LABA4_ZIP_GENERATOR_H