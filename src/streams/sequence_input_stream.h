#ifndef LABA4_READ_ONLY_STREAM_H
#define LABA4_READ_ONLY_STREAM_H

#include "../../LABA_2/sequence.h"
#include "../streams/input_stream.h"

template<class T>
class SequenceInputStream : public InputStream<T>{

private:
    Sequence<T>* source; //источник данных stream
    int position; // индекс следующего элемента
    bool opened; // флаг открыт ли stream

public:

    SequenceInputStream(Sequence<T>* source);

    void open() override;
    void close() override;

    bool is_open() const override;
    bool is_end_of_stream() const override;

    bool can_seek() const override; // возможность перепрыгнуть в другое место stream
    bool can_go_back() const override;

    int get_position() const override;

    void seek(int new_position) override;
    const T& read() override;
};

template<class T>
SequenceInputStream<T>::SequenceInputStream(Sequence<T>* source)
    : source(source), position(0), opened(false) {}

template<class T>
void SequenceInputStream<T>::open() {
    opened = true;
}

template<class T>
void SequenceInputStream<T>::close() {
    opened = false;
}

template<class T>
bool SequenceInputStream<T>::is_open() const {
    return opened;
}

template<class T>
bool SequenceInputStream<T>::is_end_of_stream() const {

    Cardinal length = source->get_length();

    if(length.is_infinite())
        return false;

    return position >= length.get_offset();
}

template<class T>
bool SequenceInputStream<T>::can_seek() const {
    return true;
}

template<class T>
bool SequenceInputStream<T>::can_go_back() const {
    return true;
}

template<class T>
int SequenceInputStream<T>::get_position() const {
    return position;
}

template<class T>
void SequenceInputStream<T>::seek(int new_position) {
    if(new_position < 0)
        throw std::out_of_range("Negative stream position");

    Cardinal length = source->get_length();

    if(!length.is_infinite() && new_position > length.get_offset())
        throw std::out_of_range("Seek position out of range");

    position = new_position;
}

template<class T>
const T& SequenceInputStream<T>::read() {
    if(!opened)
        throw std::logic_error("Stream is closed");

    if(is_end_of_stream())
        throw std::out_of_range("End of stream reached");

    return source->get(position++);
}

#endif //LABA4_READ_ONLY_STREAM_H