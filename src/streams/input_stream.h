#ifndef LABA4_INPUT_STREAM_H
#define LABA4_INPUT_STREAM_H

#include "istream.h"

template<class T>
class InputStream : public IStream<T> {

public:

    virtual bool is_end_of_stream() const = 0;

    virtual bool can_seek() const = 0;

    virtual bool can_go_back() const = 0;

    virtual void seek(int pos) = 0;

    virtual const T& read() = 0;
};

#endif //LABA4_INPUT_STREAM_H