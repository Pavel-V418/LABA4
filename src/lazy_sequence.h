#ifndef LABA4_LAZY_SEQUENCE_H
#define LABA4_LAZY_SEQUENCE_H

#include <functional>
#include "sliding_cache.h"
#include "../LABA_2/sequence.h"
#include "generators/generator.h"
#include "generators/map_generator.h"
#include "../LABA_2/mutableArraySequence.h"
#include "generators/concat_generator.h"
#include "generators/filter_generator.h"
#include "generators/take_generator.h"
#include "generators/skip_generator.h"
#include "generators/zip_generator.h"
#include "generators/append_generator.h"
#include "generators/insert_at_generator.h"
#include "generators/prepend_generator.h"
#include "utils/pair.h"

//окошко, чтобы мы возвращались назад
template<class T>
class LazySequence : public Sequence<T> {

public:
    LazySequence();
    LazySequence(std::function<Generator<T>*()> generator_factory, const Cardinal& length, int window_size);

    ~LazySequence() override;

    const T& get(int index) override;
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
    LazySequence<T>* skip(int count);

    LazySequence<T>* concat(Sequence<T>* other);
    LazySequence<T>* get_subsequence(int start,int end);

    LazySequence<T>* append(const T& item) override;
    LazySequence<T>* prepend(const T& item) override;
    LazySequence<T>* insert_at(const T& item, int index) override;
    //LazySequence<T>* remove_at(int index);

    template<class T2>
    LazySequence<Pair<T,T2>>* zip(Sequence<T2>* other);

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
    SlidingCache<T>* cache; // плавающее окно последних материализованных элементов
    Generator<T>* generator;
    Cardinal length;

    int window_size;
    int current_position; // до какого логического элемента мы догенерировали sequence
    std::function<Generator<T>*()> generator_factory; // создает генератор заново (1000->5)

    void check_range(int index) const;
};

template<class T>
LazySequence<T>::LazySequence()
    : cache(new SlidingCache<T>(100)), generator(nullptr), generator_factory(nullptr), current_position(-1),//поч -1
      length(Cardinal::infinity()),window_size(100)
{}

template<class T>
LazySequence<T>::LazySequence(std::function<Generator<T>*()> generator_factory,const Cardinal& length,int window_size)
    : cache(new SlidingCache<T>(window_size)), generator_factory(generator_factory),generator(generator_factory()),
      current_position(-1),length(length),window_size(window_size) {}

template<class T>
LazySequence<T>::~LazySequence() {
    delete generator;
    delete cache;
}

template<class T>
const T&
LazySequence<T>::get(int index) {
    check_range(index);

    if(cache->contains(index))
        return cache->get(index);
    
    if(index < current_position){
        cache->clear();

        delete generator;

        generator = generator_factory();
        current_position = -1;
    }

    while(current_position < index){
        if(!generator || !generator->has_next())
            throw std::out_of_range("Index out of range");

        T next = generator->get_next();

        cache->push(next, current_position + 1);

        current_position++;
    }

    return cache->get(index);
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
    return cache->get_count();
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

template<class T>
LazySequence<T>* LazySequence<T>::append(const T& item) {

    auto factory = [this, item]() { // функция по созданию генератора
        return new AppendGenerator<T>(this,item);
    };

    Cardinal new_length;

    if(length.is_infinite())
        new_length = Cardinal::infinity();

    else
        new_length = Cardinal(length.get_value() + 1);

    return new LazySequence<T>(factory,new_length,window_size);
}

template<class T>
LazySequence<T>* LazySequence<T>::prepend(const T& item) {
    auto factory =[this, item]() {
        return new PrependGenerator<T>(this,item);
    };

    Cardinal new_length;

    if(length.is_infinite())
        new_length = Cardinal::infinity();

    else
        new_length = Cardinal(length.get_value() + 1);

    return new LazySequence<T>(factory,new_length,window_size);
}

template<class T>
LazySequence<T>* LazySequence<T>::insert_at(const T& item,int index) {
    if(index < 0)
        throw std::out_of_range("Negative insert index");

    if(!length.is_infinite() && index > length.get_value())
        throw std::out_of_range("Insert index out of range");

    auto factory =[this, item, index]() {
        return new InsertAtGenerator<T>(this, item, index);
    };

    Cardinal new_length;

    if(length.is_infinite())
        new_length = Cardinal::infinity();

    else
        new_length = Cardinal(length.get_value() + 1);

    return new LazySequence<T>(factory,new_length,window_size);
}

template<class T>
void LazySequence<T>::append_internal(const T&) {
    throw std::logic_error("append_internal is not supported for LazySequence");
}

template<class T>
void LazySequence<T>::prepend_internal(const T& item) {
    throw std::logic_error("prepend_internal is not supported for LazySequence");
}

template<class T>
void LazySequence<T>::insert_at_internal(const T& item,int index) {
    throw std::logic_error("prepend_internal is not supported for LazySequence");
}

template<class T>
void LazySequence<T>::remove_at_internal(int index) {
    throw std::logic_error("prepend_internal is not supported for LazySequence");
}

template<class T>
LazySequence<T>* LazySequence<T>::map(std::function<T(const T&)> transform) {
    auto factory =[this, transform]() {
        return new MapGenerator<T>(this,transform);
    };

    return new LazySequence<T>(factory,get_length(),window_size);
}

template<class T>
LazySequence<T>* LazySequence<T>::where(std::function<bool(const T&)> predicate) {
    auto factory = [this, predicate]() {
        return new FilterGenerator<T>(this, predicate);
    };

    return new LazySequence<T>(factory, Cardinal::infinity(), window_size);
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
LazySequence<T>*
LazySequence<T>::take(int count) {
    if(count < 0)
        throw std::out_of_range("Negative take count");

    auto factory = [this, count]() {
        return new TakeGenerator<T>(this,count);
    };

    return new LazySequence<T>(factory,Cardinal(count),window_size);
}

template<class T>
LazySequence<T>* LazySequence<T>::skip(int count) {
    if(count < 0)
        throw std::out_of_range("Negative skip count");

    auto factory =[this, count]() {
        return new SkipGenerator<T>(this,count);
    };

    Cardinal new_length;

    if(length.is_infinite())
        new_length = Cardinal::infinity();

    else{
        int result = length.get_value() - count;

        if(result < 0)
            result = 0;

        new_length = Cardinal(result);
    }

    return new LazySequence<T>(factory,new_length,window_size);
}

template<class T>
LazySequence<T>* LazySequence<T>::concat(Sequence<T>* other) {
    auto factory =[this, other]() {
        return new ConcatGenerator<T>(this,other);
    };

    return new LazySequence<T>(factory, get_length() + other->get_length(), window_size);
}

template<class T>
LazySequence<T>* LazySequence<T>::get_subsequence(int start, int end) {
    if(start < 0 || end < start)
        throw std::out_of_range("Invalid subsequence range");

    return skip(start)->take(end - start + 1);
}

template<class T>
template<class T2>
LazySequence<Pair<T,T2>>* LazySequence<T>::zip(Sequence<T2>* other) {
    auto factory = [this, other]() {
        return new ZipGenerator<T,T2>(this,other);
    };

    Cardinal new_length;

    Cardinal first = this->get_length();

    Cardinal second = other->get_length();

    if(first.is_infinite() && second.is_infinite())
        new_length = Cardinal::infinity();

    else if(first.is_infinite())

        new_length = second;

    else if(second.is_infinite())

        new_length = first;

    else{
        int minimum;

        if(first.get_value() < second.get_value())
            minimum = first.get_value();

        else
            minimum = second.get_value();

        new_length = Cardinal(minimum);
    }

    return new LazySequence<Pair<T,T2>>(factory, new_length, window_size);
}

//private functions
template<class T>
void LazySequence<T>::check_range(int index) const {
    if(index < 0)
        throw std::out_of_range("Index out of range");

    if(!length.is_infinite() && index >= length.get_value())
        throw std::out_of_range("Index out of range");
}

#endif //LABA4_LAZY_SEQUENCE_H