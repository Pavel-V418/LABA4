#ifndef LABA4_ISTREAM_H
#define LABA4_ISTREAM_H

template<class T>
class IStream {

public:

    virtual ~IStream() = default;

    virtual void open() = 0;
    virtual void close() = 0;

    virtual bool is_open() const = 0;
    virtual int get_position() const = 0;
};

#endif //LABA4_ISTREAM_H
