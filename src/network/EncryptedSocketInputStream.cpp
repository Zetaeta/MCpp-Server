
#include <string.h>
#include <iostream>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>

#include <openssl/evp.h>

#include "EncryptedSocketInputStream.hpp"
#include "NetUtils.hpp"
#include "util/Unicode.hpp"
#include "logging/Logger.hpp"
#include "MinecraftServer.hpp"
#include "util/AutoDeleter.hpp"
#include "util/Utils.hpp"

USING_LOGGING_LEVEL

using std::vector;
using std::cout;

namespace MCServer {
namespace Network {

EncryptedSocketInputStream::EncryptedSocketInputStream(int socketfd, EVP_CIPHER_CTX *decryptor)
:socketfd(socketfd), decryptor(decryptor), input(CIPHER_BUFFER_LENGTH), output(CIPHER_BUFFER_LENGTH) {
   cout << "EncryptedSocketInputStream(int, EVP_CIPHER_CTX *): input.fullSize() = " << input.fullSize() << ", output.fullSize() = " << output.fullSize() << '\n';
}

EncryptedSocketInputStream & EncryptedSocketInputStream::operator>>(uint8_t &data) {
    Logging::Logger &log = MinecraftServer::getServer().getLogger();
    log << "EncryptedSocketInputStream::operator>>\n";
    read(&data, sizeof(data));
    log << "Read data!\n";
    return *this;
}

EncryptedSocketInputStream & EncryptedSocketInputStream::operator>>(int8_t &data) {
    read(&data, sizeof(data));
    return *this;
}

EncryptedSocketInputStream & EncryptedSocketInputStream::operator>>(uint16_t &data) {
    read(&data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

EncryptedSocketInputStream & EncryptedSocketInputStream::operator>>(int16_t &data) {
    read(&data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

EncryptedSocketInputStream & EncryptedSocketInputStream::operator>>(uint32_t &data) {
    read(&data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

EncryptedSocketInputStream & EncryptedSocketInputStream::operator>>(int32_t &data) {
    read(&data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

EncryptedSocketInputStream & EncryptedSocketInputStream::operator>>(uint64_t &data) {
    read(&data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

EncryptedSocketInputStream & EncryptedSocketInputStream::operator>>(int64_t &data) {
    read(&data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

EncryptedSocketInputStream & EncryptedSocketInputStream::operator>>(float &data) {
    read(&data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

EncryptedSocketInputStream & EncryptedSocketInputStream::operator>>(double &data) {
    read(&data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

EncryptedSocketInputStream & EncryptedSocketInputStream::operator>>(std::string &data) {
    uint16_t length;
    operator>>(length);
    vector<uint16_t> ucs2(length);
    read(ucs2.data(), length * 2);
    for (auto it = ucs2.begin(); it != ucs2.end(); ++it) {
        uint16_t u = *it;
        *it = (u << 8) | (u >> 8);
    }
    data = ucs2ToUtf8(ucs2);
    return *this;
}

/*
EncryptedSocketInputStream & EncryptedSocketInputStream::peek(uint8_t &data) {
    Logging::Logger &log = MinecraftServer::getServer().getLogger();
    recv(socketfd, &data, sizeof(data), MSG_PEEK);
    return *this;
}

EncryptedSocketInputStream & EncryptedSocketInputStream::peek(int8_t &data) {
    recv(socketfd, &data, sizeof(data), MSG_PEEK);
    return *this;
}

EncryptedSocketInputStream & EncryptedSocketInputStream::peek(uint16_t &data) {
    recv(socketfd, &data, sizeof(data), MSG_PEEK);
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

EncryptedSocketInputStream & EncryptedSocketInputStream::peek(int16_t &data) {
    recv(socketfd, &data, sizeof(data), MSG_PEEK);
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

EncryptedSocketInputStream & EncryptedSocketInputStream::peek(uint32_t &data) {
    recv(socketfd, &data, sizeof(data), MSG_PEEK);
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

EncryptedSocketInputStream & EncryptedSocketInputStream::peek(int32_t &data) {
    recv(socketfd, &data, sizeof(data), MSG_PEEK);
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

EncryptedSocketInputStream & EncryptedSocketInputStream::peek(uint64_t &data) {
    recv(socketfd, &data, sizeof(data), MSG_PEEK);
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

EncryptedSocketInputStream & EncryptedSocketInputStream::peek(int64_t &data) {
    recv(socketfd, &data, sizeof(data), MSG_PEEK);
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

EncryptedSocketInputStream & EncryptedSocketInputStream::peek(float &data) {
    recv(socketfd, &data, sizeof(data), MSG_PEEK);
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

EncryptedSocketInputStream & EncryptedSocketInputStream::peek(double &data) {
    recv(socketfd, &data, sizeof(data), MSG_PEEK);
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

EncryptedSocketInputStream & EncryptedSocketInputStream::peek(std::string &data) {
    uint16_t length;
    peek(length);
    vector<uint16_t> ucs2(length);
    uint16_t *rawdata = new uint16_t[length + 1];
    recv(socketfd, rawdata, (length * 2) + 2, MSG_PEEK);
    memcpy(ucs2.data(), rawdata + 1, length * 2);
    for (auto it = ucs2.begin(); it != ucs2.end(); ++it) {
        uint16_t u = *it;
        *it = (u << 8) | (u >> 8);
    }
    data = ucs2ToUtf8(ucs2);
    return *this;
}
*/

void * EncryptedSocketInputStream::readRaw(void *data, size_t length) {
    read(data, length);
    return data;
}

void * EncryptedSocketInputStream::readRaw(size_t length) {
    char * data = new char[length];
    read(data, length);
    return data;
}


ssize_t EncryptedSocketInputStream::read(void *buf, size_t count) {
    size_t added = 0;
    while (added < count) { // Buf is not full
        if (output.available() < (count - added)) {
            if (output.spaceBefore() > output.spaceAfter()) {
                output.shiftToStart();
            }
            if (input.available() == 0) { // Input buffer is empty.
                populateInput();
            }
            int outLen, inLen = min(input.available(), output.spaceAfter() - decryptor->cipher->block_size);
            EVP_DecryptUpdate(decryptor, output.end(), &outLen, input.begin(), inLen);
            input.take(inLen);
            output.add(outLen);
        }
        int copy = min(count - added, output.available());
        memcpy(buf, output.begin(), copy);
        output.take(copy);
        added += copy;
    }
    return added;
}

void EncryptedSocketInputStream::populateInput() {
    input.shiftToStart();
    ssize_t bytesRead = ::read(socketfd, input.end(), input.spaceAfter());
    if (bytesRead < 0) {
        std::cerr << "bytesRead < 0!\n";
        return;
    }
    input.add(bytesRead);
}

}
}

