#ifndef LABA4_TAKE_GENERATOR_H
#define LABA4_TAKE_GENERATOR_H

#include "generator.h"
#include "../../LABA_2/sequence.h"

// генератор - ограничитель
template<class T>
class TakeGenerator : public Generator<T> {

private:
    Sequence<T>* source; // исходная sequence

    int limit; // сколько элементов разрешено выдать
    int produced; // сколько уже выдано (понимание, когда остановиться)

public:

    TakeGenerator(Sequence<T>* source,int limit);

    T get_next() override;
    T get(const Cardinal& index) override;

    bool has_next() const override;
};

template<class T>
TakeGenerator<T>::TakeGenerator(Sequence<T>* source,int limit)
    : source(source), limit(limit), produced(0) {}

template<class T>
T TakeGenerator<T>::get_next() {
    return source->get(produced++);
}

template<class T>
bool TakeGenerator<T>::has_next() const {
    return produced < limit;
}

template<class T>
T TakeGenerator<T>::get(const Cardinal& index) {

    if(index.get_omega_count() != 0)
        throw std::logic_error(
            "TakeGenerator does not support omega indices"
        );

    if(index.get_offset() >= limit)
        throw std::out_of_range(
            "TakeGenerator: index out of range"
        );

    return source->get(index.get_offset());
}

#endif