#ifndef LABA4_LAZY_SEQUENCE_H
#define LABA4_LAZY_SEQUENCE_H

#include "../LABA_2/sequence.h"
#include "generators/generator.h"
#include "generators/map_generator.h"
#include "../LABA_2/mutableArraySequence.h"
#include "generators/concat_generator.h"
#include "generators/filter_generator.h"
#include "generators/take_generator.h"
//окошко, чтобы мы возвращались назад

template<class T>
class LazySequence : public Sequence<T> {

public:
    LazySequence();
    LazySequence(Generator<T> *generator, Sequence<T> *init_seq, const Cardinal& length);

    ~LazySequence() override;

    const T& get(int index);
    const T& get_first() override;
    const T& get_last() override;

    Cardinal get_length() const override;
    int get_materialized_count() const;

    IEnumerator<T>* get_enumerator() const override;

    Sequence<T>* instance() override;
    Sequence<T>* create_empty_sequence() const override;

    LazySequence<T>* map(std::function<T(const T&)> transform);
    LazySequence<T>* where(std::function<bool(const T&)> predicate);
    T reduce(std::function<T(const T&, const T&)> reducer,T initial);

    LazySequence<T>* take(int count);

    LazySequence<T>* concat(Sequence<T>* other);

    class LazyEnumerator : public IEnumerator<T> {

    private:
        LazySequence<T>* sequence;
        int current_index;

    public:
        LazyEnumerator(LazySequence<T>* sequence)
            : sequence(sequence), current_index(0) {}

        bool has_more_elements() override {

            Cardinal length = sequence->get_length();

            if(length.is_infinite())
                return true;

            return current_index < length.get_value();
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
    Cardinal length;

    static void check_range(int index);
};

template<class T>
LazySequence<T>::LazySequence()
    : materialized(nullptr), generator(nullptr) {}

template<class T>
LazySequence<T>::LazySequence(Generator<T> *generator, Sequence<T> *init_seq, const Cardinal& length)
    : materialized(init_seq), generator(generator), length(length) {}

template<class T>
LazySequence<T>::~LazySequence() {
    delete generator;
    delete materialized;
}

template<class T>
const T& LazySequence<T>::get(int index) { // материализацию элементов по требованию
    check_range(index);

    while (materialized->get_length().get_value() <= index) { // пока нужно элемента нет в cache - генерируй его

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
    if(length.is_infinite())
        throw std::logic_error("Infinite sequence has no last element");

    return get(length.get_value() - 1);
}

template<class T>
Cardinal LazySequence<T>::get_length() const{
    return length;
}

template<class T>
int LazySequence<T>::get_materialized_count() const {
    return materialized->get_length().get_value();
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

    return new LazySequence<T>(gen,cache,get_length());
}

template<class T>
LazySequence<T>* LazySequence<T>::where(std::function<bool(const T&)> predicate) {

    Generator<T>* gen = new FilterGenerator<T>(this,predicate);

    Sequence<T>* cache = new MutableArraySequence<T>();

    return new LazySequence<T>(gen,cache,Cardinal::infinity());
}

template<class T>
T LazySequence<T>::reduce(std::function<T(const T&, const T&)> reducer,T initial) {

    T result = initial;

    IEnumerator<T>* enumerator = get_enumerator();

    while(enumerator->has_more_elements())
        result = reducer(result,enumerator->next());

    delete enumerator;

    return result;
}

template<class T>
LazySequence<T>* LazySequence<T>::take(int count) {
    if(count < 0)
        throw std::out_of_range("Negative take count");

    Generator<T>* gen = new TakeGenerator<T>(this,count);
    Sequence<T>* cache = new MutableArraySequence<T>();

    return new LazySequence<T>(gen,cache,Cardinal(count));
}

template<class T>
LazySequence<T>* LazySequence<T>::concat(Sequence<T>* other) {

    Generator<T>* gen = new ConcatGenerator<T>(this,other);
    Sequence<T>* cache = new MutableArraySequence<T>();

    return new LazySequence<T>(gen,cache,get_length() + other->get_length());
}

//private functions
template<class T>
void LazySequence<T>::check_range(int index) {
    if (index < 0)
        throw std::out_of_range("Index out of range");
}


#endif //LABA4_LAZY_SEQUENCE_H
