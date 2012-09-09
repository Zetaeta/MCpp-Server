
#ifndef PLAINSOCKETINPUTSTREAM_HPP
#define PLAINSOCKETINPUTSTREAM_HPP

#include <string>
#include <stdint.h>

#include "SocketInputStream.hpp"

namespace MCServer {
namespace Network {

class PlainSocketInputStream : public virtual SocketInputStream {
public:
    PlainSocketInputStream(int socketfd);
    PlainSocketInputStream & operator>>(uint8_t&);
    PlainSocketInputStream & operator>>(int8_t&);
    PlainSocketInputStream & operator>>(uint16_t&);
    PlainSocketInputStream & operator>>(int16_t&);
    PlainSocketInputStream & operator>>(uint32_t&);
    PlainSocketInputStream & operator>>(int32_t&);
    PlainSocketInputStream & operator>>(uint64_t&);
    PlainSocketInputStream & operator>>(int64_t&);
    PlainSocketInputStream & operator>>(std::string &);
    PlainSocketInputStream & operator>>(float &);
    PlainSocketInputStream & operator>>(double &);

    PlainSocketInputStream & peek(uint8_t&);
    PlainSocketInputStream & peek(int8_t&);
    PlainSocketInputStream & peek(uint16_t&);
    PlainSocketInputStream & peek(int16_t&);
    PlainSocketInputStream & peek(uint32_t&);
    PlainSocketInputStream & peek(int32_t&);
    PlainSocketInputStream & peek(uint64_t&);
    PlainSocketInputStream & peek(int64_t&);
    PlainSocketInputStream & peek(std::string &);
    PlainSocketInputStream & peek(float &);
    PlainSocketInputStream & peek(double &);

//    template<class T>
//    T * read(T *data, size_t length) {
//        ::read(socketfd, data, sizeof(T) * length);
//        return data;
//    }

    /**
     * Return value mut be <code>delete</code>d
     */
//    template<class T>
//    T * read(size_t length) {
//        T * data = new T[length];
//        ::read(socketfd, data, sizeof(T) * length);
//        return data;
//    }

    void * readRaw(void *data, size_t length);

    /**
     * Return value mut be <code>delete[]</code>d
     */
    void * readRaw(size_t length);
private:
    int socketfd;
};

}
}

#endif
