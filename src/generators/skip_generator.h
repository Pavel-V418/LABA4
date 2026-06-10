#ifndef LABA4_SKIP_GENERATOR_H
#define LABA4_SKIP_GENERATOR_H

#include "../generators/generator.h"

template<class T>
class SkipGenerator : public Generator<T>{

private:
    Sequence<T>* source;

    int skip_count;
    int current_index;

public:
    SkipGenerator(Sequence<T>* source, int skip_count);

    T get_next() override;
    T get(const Cardinal& index) override;

    bool has_next() const override;

};

template<class T>
SkipGenerator<T>::SkipGenerator(Sequence<T>* source,int skip_count)
    : source(source), skip_count(skip_count), current_index(skip_count) {}

template<class T>
T SkipGenerator<T>::get_next() {
    return source->get(current_index++);
}

template<class T>
bool SkipGenerator<T>::has_next() const {
    Cardinal length = source->get_length();

    if (length.is_infinite())
        return true;

    return current_index < length.get_offset();
}

template<class T>
T SkipGenerator<T>::get(const Cardinal& index)
{
    return source->get(
        Cardinal(
            index.get_omega_count(),
            index.get_offset() + skip_count
        )
    );
}

#endif //LABA4_SKIP_GENERATOR_H
