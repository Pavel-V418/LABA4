#ifndef LABA4_GENERATOR_H
#define LABA4_GENERATOR_H

#include "../../LABA_2/sequence.h"
#include <functional>

// отвечает за то, как вычислить сл элемент
// хранит состояние генерации, вычисляет следующий элемент, помнит контекст
template<class T>
class Generator {

public:
    virtual ~Generator() {}

    virtual T get_next() = 0; // генерирует следущий элемент
    virtual T get(const Cardinal& index) = 0;
    virtual bool has_next() const = 0; // нужен для конечных последовательностей
};

#endif //LABA4_GENERATOR_H
