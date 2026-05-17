#ifndef LABA4_LAZY_SEQUENCE_H
#define LABA4_LAZY_SEQUENCE_H

#include "LABA_2/sequence.h"
#include "src/generators/generator.h"
#include "src/generators/map_generator.h"
#include "LABA_2/mutableArraySequence.h"
#include "src/generators/filter_generator.h"

template<class T>
class LazySequence : public Sequence<T> {

public:
    LazySequence();
    LazySequence(Generator<T>* generator, Sequence<T>* init_seq, bool infinite = true);

    ~LazySequence() override;

    const T& get(int index);
    const T& get_first() override;
    const T& get_last() override;

    int get_length() const override;
    int get_materialized_count() const;

    IEnumerator<T>* get_enumerator() const override;

    Sequence<T>* instance() override;
    Sequence<T>* create_empty_sequence() const override;

    LazySequence<T>* map(std::function<T(const T&)> transform);
    LazySequence<T>* where(std::function<bool(const T&)> predicate);

    class LazyEnumerator : public IEnumerator<T> {

    private:
        LazySequence<T>* sequence;
        int current_index;

    public:
        LazyEnumerator(LazySequence<T>* sequence)
            : sequence(sequence), current_index(0) {}

        bool has_more_elements() override {
            if(sequence->infinite)
                return true;

            return current_index < sequence->get_length();
        }

        const T& next() override{
            return sequence->get(current_index++);
        }
    };

protected:

    void append_internal(const T& item) override;
    void prepend_internal(const T& item) override;
    void insert_at_internal(const T& item,int index) override;
    void remove_at_internal(int index) override;

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
const T& LazySequence<T>::get(int index) { // материализацию элементов по требованию
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
const T& LazySequence<T>::get_first() {
    return get(0);
}

template<class T>
const T &LazySequence<T>::get_last() {
    check_infinite();

    return materialized->get_last();
}

template<class T>
int LazySequence<T>::get_length() const{
    check_infinite();
    
    return materialized->get_length();
}

template<class T>
int LazySequence<T>::get_materialized_count() const {

    return materialized->get_length();
}

template<class T>
IEnumerator<T>* LazySequence<T>::get_enumerator() const {
    return new LazyEnumerator(const_cast<LazySequence<T>*>(this)); // как
}

template<class T>
Sequence<T>* LazySequence<T>::instance() { // мы обязаны были добавить, так как она есть в Sequence
    throw std::logic_error("LazySequence copying is not implemented"); // у нас нет логики mut/immut
}

template<class T>
Sequence<T>* LazySequence<T>::create_empty_sequence() const {
    return new MutableArraySequence<T>();
}

// internal methods
template<class T>
void LazySequence<T>::append_internal(const T& item) {
    materialized = materialized->append(item);
}

template<class T>
void LazySequence<T>::prepend_internal(const T& item) {
    materialized = materialized->prepend(item);
}

template<class T>
void LazySequence<T>::insert_at_internal(const T& item,int index) {
    materialized = materialized->insert_at(item, index);
}

template<class T>
void LazySequence<T>::remove_at_internal(int index) {
    materialized = materialized->remove_at(index);
}

template<class T>
LazySequence<T>* LazySequence<T>::map(std::function<T(const T&)> transform) {

    Generator<T>* gen = new MapGenerator<T>(this,transform);

    auto* cache = new MutableArraySequence<T>();

    return new LazySequence<T>(gen,cache,infinite);
}

template<class T>
LazySequence<T>* LazySequence<T>::where(std::function<bool(const T&)> predicate) {

    Generator<T>* gen = new FilterGenerator<T>(this,predicate);

    Sequence<T>* cache = new MutableArraySequence<T>();

    return new LazySequence<T>(gen,cache,infinite);
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
