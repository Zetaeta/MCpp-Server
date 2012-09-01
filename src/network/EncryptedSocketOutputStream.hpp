
#ifndef SOCKETOUTPUTSTREAM_HPP
#define SOCKETOUTPUTSTREAM_HPP

#include <string>
#include <stdint.h>

extern "C" ssize_t write(int fd, const void *buf, size_t count);

namespace MCServer {
namespace Network {

#ifndef PACKET_HPP
class Packet;
#endif

class EncryptedSocketOutputStream {
public:
    EncryptedSocketOutputStream(int socketfd, EVP_CIPHER_CTX decryptor);
    EncryptedSocketOutputStream & operator<<(uint8_t);
    EncryptedSocketOutputStream & operator<<(int8_t);
    EncryptedSocketOutputStream & operator<<(uint16_t);
    EncryptedSocketOutputStream & operator<<(int16_t);
    EncryptedSocketOutputStream & operator<<(uint32_t);
    EncryptedSocketOutputStream & operator<<(int32_t);
    EncryptedSocketOutputStream & operator<<(uint64_t);
    EncryptedSocketOutputStream & operator<<(int64_t);
    EncryptedSocketOutputStream & operator<<(float);
    EncryptedSocketOutputStream & operator<<(double);
    EncryptedSocketOutputStream & operator<<(const std::string &);
    EncryptedSocketOutputStream & operator<<(const Packet &);

    template<class T>
    void write(const T *data, size_t length) {
        write(socketfd, data, sizeof(T) * length);
    }

    void writeRaw(const void *data, size_t length);

private:
    int socketfd;
    static ssize_t write(int fd, const void *buf, size_t count);
};

}
}

#endif
