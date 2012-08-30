
#ifndef SOCKETINPUTSTREAM_HPP
#define SOCKETINPUTSTREAM_HPP

#include <string>
#include <stdint.h>

extern "C" ssize_t read(int fd, void *buf, size_t count);

namespace MCServer {
namespace Network {

class SocketInputStream {
public:
    SocketInputStream(int socketfd);
    SocketInputStream & operator>>(uint8_t&);
    SocketInputStream & operator>>(int8_t&);
    SocketInputStream & operator>>(uint16_t&);
    SocketInputStream & operator>>(int16_t&);
    SocketInputStream & operator>>(uint32_t&);
    SocketInputStream & operator>>(int32_t&);
    SocketInputStream & operator>>(uint64_t&);
    SocketInputStream & operator>>(int64_t&);
    SocketInputStream & operator>>(std::string &);
    SocketInputStream & operator>>(float &);
    SocketInputStream & operator>>(double &);

    SocketInputStream & peek(uint8_t&);
    SocketInputStream & peek(int8_t&);
    SocketInputStream & peek(uint16_t&);
    SocketInputStream & peek(int16_t&);
    SocketInputStream & peek(uint32_t&);
    SocketInputStream & peek(int32_t&);
    SocketInputStream & peek(uint64_t&);
    SocketInputStream & peek(int64_t&);
    SocketInputStream & peek(std::string &);
    SocketInputStream & peek(float &);
    SocketInputStream & peek(double &);

    template<class T>
    T * read(T *data, size_t length) {
        ::read(socketfd, data, sizeof(T) * length);
        return data;
    }

    /**
     * Return value mut be <code>delete</code>d
     */
    template<class T>
    T * read(size_t length) {
        T * data = new T[length];
        ::read(socketfd, data, sizeof(T) * length);
        return data;
    }

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
