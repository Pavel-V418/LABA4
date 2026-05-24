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

#endif