#ifndef LABA4_OUTPUT_STREAM_H
#define LABA4_OUTPUT_STREAM_H

#include "istream.h"

template<class T>
class OutputStream : public IStream<T> {

public:

    virtual void write(const T& item) = 0;
};

#endif //LABA4_OUTPUT_STREAM_H