#ifndef LABA4_WRITE_ONLY_STREAM_H
#define LABA4_WRITE_ONLY_STREAM_H

#include "../../LABA_2/sequence.h"
#include "output_stream.h"

template<class T>
class SequenceOutputStream : public OutputStream<T>{

private:
    Sequence<T>* target; // куда пишем элементы

    int position;
    bool opened;

public:

    SequenceOutputStream(Sequence<T>* target);

    void open() override;
    void close() override;

    bool is_open() const override;

    int get_position() const override;

    void write(const T& item) override;
};

template<class T>
SequenceOutputStream<T>::SequenceOutputStream(Sequence<T>* target)
    : target(target), position(0), opened(false) {}

template<class T>
void SequenceOutputStream<T>::open() {
    opened = true;
}

template<class T>
void SequenceOutputStream<T>::close() {
    opened = false;
}

template<class T>
bool SequenceOutputStream<T>::is_open() const {
    return opened;
}

template<class T>
int SequenceOutputStream<T>::get_position() const {
    return position;
}

template<class T>
void SequenceOutputStream<T>::write(const T& item) {
    if(!opened)
        throw std::logic_error("Stream is closed");

    target = target->append(item);

    position++;
}

#endif //LABA4_WRITE_ONLY_STREAM_H