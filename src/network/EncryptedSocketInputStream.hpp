
#ifndef ENCRYPTEDSOCKETINPUTSTREAM_HPP
#define ENCRYPTEDSOCKETINPUTSTREAM_HPP

#include <string>
#include <stdint.h>
#include <vector>

#include "SocketInputStream.hpp"

extern "C" ssize_t read(int fd, void *buf, size_t count);

#ifndef CIPHER_BUFFER_LENGTH
#define CIPHER_BUFFER_LENGTH 1024
#endif

#ifndef HEADER_ENVELOPE_H
typedef struct evp_cipher_ctx_st EVP_CIPHER_CTX;
#endif

namespace MCServer {
namespace Network {

class EncryptedSocketInputStream : public virtual SocketInputStream {
public:
    EncryptedSocketInputStream(int socketfd, EVP_CIPHER_CTX *);
    EncryptedSocketInputStream() {}
    EncryptedSocketInputStream & operator>>(uint8_t&);
    EncryptedSocketInputStream & operator>>(int8_t&);
    EncryptedSocketInputStream & operator>>(uint16_t&);
    EncryptedSocketInputStream & operator>>(int16_t&);
    EncryptedSocketInputStream & operator>>(uint32_t&);
    EncryptedSocketInputStream & operator>>(int32_t&);
    EncryptedSocketInputStream & operator>>(uint64_t&);
    EncryptedSocketInputStream & operator>>(int64_t&);
    EncryptedSocketInputStream & operator>>(std::string &);
    EncryptedSocketInputStream & operator>>(float &);
    EncryptedSocketInputStream & operator>>(double &);

    EncryptedSocketInputStream & peek(uint8_t&);
    EncryptedSocketInputStream & peek(int8_t&);
    EncryptedSocketInputStream & peek(uint16_t&);
    EncryptedSocketInputStream & peek(int16_t&);
    EncryptedSocketInputStream & peek(uint32_t&);
    EncryptedSocketInputStream & peek(int32_t&);
    EncryptedSocketInputStream & peek(uint64_t&);
    EncryptedSocketInputStream & peek(int64_t&);
    EncryptedSocketInputStream & peek(std::string &);
    EncryptedSocketInputStream & peek(float &);
    EncryptedSocketInputStream & peek(double &);

    ssize_t read(void *buf, size_t count);

//    template<class T>
//    inline T * read(T *data, size_t length) {
//        read(data, sizeof(T) * length);
//        return data;
//    }

    /**
     * Return value mut be <code>delete</code>d
     */
//    template<class T>
//    inline T * read(size_t length) {
//        T * data = new T[length];
//        read(data, sizeof(T) * length);
//        return data;
//    }

    void * readRaw(void *data, size_t length);

    /**
     * Return value mut be <code>delete[]</code>d
     */
    void * readRaw(size_t length);
private:
    int socketfd;
    EVP_CIPHER_CTX *decryptor;
    uint8_t encryptedBuffer[CIPHER_BUFFER_LENGTH];
    int bufferPos;
    int bufferFullLength;
    std::vector<uint8_t> outputBuffer;
    int outputBufferPos;
    size_t outputBufferFullLength;
    
};

}
}

#endif
