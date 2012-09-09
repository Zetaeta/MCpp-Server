
#ifndef SOCKETINPUTSTREAM_HPP
#define SOCKETINPUTSTREAM_HPP

#include <string>
#include <stdint.h>

extern "C" ssize_t read(int fd, void *buf, size_t count);

namespace MCServer {
namespace Network {

class SocketInputStream {
public:
    virtual SocketInputStream & operator>>(uint8_t&) = 0;
    virtual SocketInputStream & operator>>(int8_t&) = 0;
    virtual SocketInputStream & operator>>(uint16_t&) = 0;
    virtual SocketInputStream & operator>>(int16_t&) = 0;
    virtual SocketInputStream & operator>>(uint32_t&) = 0;
    virtual SocketInputStream & operator>>(int32_t&) = 0;
    virtual SocketInputStream & operator>>(uint64_t&) = 0;
    virtual SocketInputStream & operator>>(int64_t&) = 0;
    virtual SocketInputStream & operator>>(std::string &) = 0;
    virtual SocketInputStream & operator>>(float &) = 0;
    virtual SocketInputStream & operator>>(double &) = 0;

//    virtual SocketInputStream & peek(uint8_t&) = 0;
//    virtual SocketInputStream & peek(int8_t&) = 0;
//    virtual SocketInputStream & peek(uint16_t&) = 0;
//    virtual SocketInputStream & peek(int16_t&) = 0;
//    virtual SocketInputStream & peek(uint32_t&) = 0;
//    virtual SocketInputStream & peek(int32_t&) = 0;
//    virtual SocketInputStream & peek(uint64_t&) = 0;
//    virtual SocketInputStream & peek(int64_t&) = 0;
//    virtual SocketInputStream & peek(std::string &) = 0;
//    virtual SocketInputStream & peek(float &) = 0;
//    virtual SocketInputStream & peek(double &) = 0;

    template<class T>
    T * read(T *data, size_t length) {
        readRaw(data, sizeof(T) * length);
        return data;
    }

    /**
     * Return value mut be <code>delete</code>d
     */
    template<class T>
    T * read(size_t length) {
        T * data = new T[length];
        readRaw(data, sizeof(T) * length);
        return data;
    }

    virtual void * readRaw(void *data, size_t length) = 0;

    /**
     * Return value mut be <code>delete[]</code>d
     */
    virtual void * readRaw(size_t length) = 0;
};

}
}

#endif
