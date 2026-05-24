#ifndef LABA4_READ_ONLY_STREAM_H
#define LABA4_READ_ONLY_STREAM_H

#include "../../LABA_2/sequence.h"
#include "../streams/istream.h"

template<class T>
class ReadOnlyStream : public IStream<T>{

private:
    Sequence<T>* source; //источник данных stream
    int position; // индекс следующего элемента
    bool opened; // флаг открыт ли stream

public:

    ReadOnlyStream(Sequence<T>* source);

    void open() override;
    void close() override;

    bool is_open() const override;
    bool is_end_of_stream() const;

    bool can_seek() const override; // возможность перепрыгнуть в другое место stream
    bool can_go_back() const override;

    int get_position() const override;

    void seek(int new_position) override;
    const T& read();
};

template<class T>
ReadOnlyStream<T>::ReadOnlyStream(Sequence<T>* source)
    : source(source), position(0), opened(false) {}

template<class T>
void ReadOnlyStream<T>::open() {
    opened = true;
}

template<class T>
void ReadOnlyStream<T>::close() {
    opened = false;
}

template<class T>
bool ReadOnlyStream<T>::is_open() const {
    return opened;
}

template<class T>
bool ReadOnlyStream<T>::is_end_of_stream() const {

    Cardinal length = source->get_length();

    if(length.is_infinite())
        return false;

    return position >= length.get_value();
}

template<class T>
bool ReadOnlyStream<T>::can_seek() const {
    return true;
}

template<class T>
bool ReadOnlyStream<T>::can_go_back() const {
    return true;
}

template<class T>
int ReadOnlyStream<T>::get_position() const {
    return position;
}

template<class T>
void ReadOnlyStream<T>::seek(int new_position) {
    if(new_position < 0)
        throw std::out_of_range("Negative stream position");

    Cardinal length = source->get_length();

    if(!length.is_infinite() && new_position > length.get_value())
        throw std::out_of_range("Seek position out of range");

    position = new_position;
}

template<class T>
const T& ReadOnlyStream<T>::read() {
    if(!opened)
        throw std::logic_error("Stream is closed");

    if(is_end_of_stream())
        throw std::out_of_range("End of stream reached");

    return source->get(position++);
}

#endif //LABA4_READ_ONLY_STREAM_H