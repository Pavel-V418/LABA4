#ifndef LABA4_MAP_GENERATOR_H
#define LABA4_MAP_GENERATOR_H

#include "generator.h"
#include "../lazy/cardinal.h"
template<class T>
class MapGenerator : public Generator<T> { // строит посл-ть поверх старой (чтобы не вычислять всю посл-ть заранее)

public:
    MapGenerator(Sequence<T>* source,std::function<T(const T&)> transform);

    T get_next() override;
    T get(const Cardinal& index) override;

    bool has_next() const override;

private:
    Sequence<T>* source; // исходная посл-ть
    std::function<T(const T&)> transform; // правило преобразования
    int current_index;

};

template<class T>
MapGenerator<T>::MapGenerator(Sequence<T> *source, std::function<T(const T &)> transform)
    : source(source), transform(transform), current_index(0) {}

template<class T>
T MapGenerator<T>::get_next() {

    T value = source->get(current_index);

    current_index++;

    return transform(value); // результат создаётся => возвр T, а не const T&
}

template<class T>
bool MapGenerator<T>::has_next() const {
    Cardinal length = source->get_length();

    if(length.is_infinite())
        return true;

    return current_index < length.get_offset();
}

template<class T>
T MapGenerator<T>::get(const Cardinal& index) {

    return transform(source->get(index));
}

#endif //LABA4_MAP_GENERATOR_H
