#ifndef LABA4_LAZY_SEQUENCE_H
#define LABA4_LAZY_SEQUENCE_H

#include "LABA_2/sequence.h"
#include "generator.h"

template<class T>
class LazySequence : public Sequence<T> {

public:
    LazySequence();
    LazySequence(Generator<T>* generator, Sequence<T>* init_seq, bool infinite = true);

    ~LazySequence() override;

    const T& get(int index) const override;
    const T& get_first() const override;
    const T& get_last() const override;

    int get_length() const override;

private:
    Sequence<T>* materialized;
    Generator<T>* generator;
    bool infinite;

    static void check_range(int index);
    void check_infinite() const;
};

template<class T>
LazySequence<T>::LazySequence()
    : materialized(nullptr), generator(nullptr), infinite(false) {}

template<class T>
LazySequence<T>::LazySequence(Generator<T> *generator, Sequence<T> *init_seq, bool infinite)
    : materialized(init_seq), generator(generator), infinite(infinite) {}

template<class T>
LazySequence<T>::~LazySequence() {
    delete generator;
    delete materialized;
}

template<class T>
const T& LazySequence<T>::get(int index) const{ // материализацию элементов по требованию
    check_range(index);

    while (materialized->get_length() <= index) { // пока нужно элемента нет в cache - генерируй его

        if (!generator->has_next())
            throw std::out_of_range("Index out of range");

        T next = generator->get_next();

        materialized = materialized->append(next);
    }

    return materialized->get(index);
}

template<class T>
const T& LazySequence<T>::get_first() const{
    return get(0);
}

template<class T>
const T &LazySequence<T>::get_last() const {
    check_infinite();

    return materialized->get_last();
}

template<class T>
int LazySequence<T>::get_length() const{
    check_infinite();
    
    return materialized->get_length();
}

//private functions
template<class T>
void LazySequence<T>::check_range(int index) {
    if (index < 0)
        throw std::out_of_range("Index out of range");
}

template<class T>
void LazySequence<T>::check_infinite() const {
    if (infinite)
        throw std::logic_error("Infinite sequence has no last element");
}

#endif //LABA4_LAZY_SEQUENCE_H
