
#include <string.h>

#include <unistd.h>
#include <sys/socket.h>

#include "EncryptedSocketInputStream.hpp"
#include "NetUtils.hpp"
#include "../Unicode.hpp"
#include "../logging/Logger.hpp"
#include "../MinecraftServer.hpp"
#include "../AutoDeleter.hpp"


using std::vector;

namespace MCServer {
namespace Network {

EncryptedSocketInputStream::EncryptedSocketInputStream(int socketfd, EVP_CIPHER_CTX *decryptor)
    :socketfd(socketfd), decryptor(decryptor), outputBufferPos(0), bufferPos(0) {
    outputBuffer.reserve(CIPHER_BUFFER_LENGTH + decryptor->block_size);
}

EncryptedSocketInputStream & EncryptedSocketInputStream::operator>>(uint8_t &data) {
    Logging::Logger &log = MinecraftServer::getServer().getLogger();
    log << "EncryptedSocketInputStream::operator>>\n";
    ::read(socketfd, &data, sizeof(data));
    log << "Read data!\n";
    return *this;
}

EncryptedSocketInputStream & EncryptedSocketInputStream::operator>>(int8_t &data) {
    ::read(socketfd, &data, sizeof(data));
    return *this;
}

EncryptedSocketInputStream & EncryptedSocketInputStream::operator>>(uint16_t &data) {
    ::read(socketfd, &data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

EncryptedSocketInputStream & EncryptedSocketInputStream::operator>>(int16_t &data) {
    ::read(socketfd, &data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

EncryptedSocketInputStream & EncryptedSocketInputStream::operator>>(uint32_t &data) {
    ::read(socketfd, &data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

EncryptedSocketInputStream & EncryptedSocketInputStream::operator>>(int32_t &data) {
    ::read(socketfd, &data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

EncryptedSocketInputStream & EncryptedSocketInputStream::operator>>(uint64_t &data) {
    ::read(socketfd, &data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

EncryptedSocketInputStream & EncryptedSocketInputStream::operator>>(int64_t &data) {
    ::read(socketfd, &data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

EncryptedSocketInputStream & EncryptedSocketInputStream::operator>>(float &data) {
    ::read(socketfd, &data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

EncryptedSocketInputStream & EncryptedSocketInputStream::operator>>(double &data) {
    ::read(socketfd, &data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

EncryptedSocketInputStream & EncryptedSocketInputStream::operator>>(std::string &data) {
    uint16_t length;
    operator>>(length);
    vector<uint16_t> ucs2(length);
    ::read(socketfd, ucs2.data(), length * 2);
    for (auto it = ucs2.begin(); it != ucs2.end(); ++it) {
        uint16_t u = *it;
        *it = (u << 8) | (u >> 8);
    }
    data = ucs2ToUtf8(ucs2);
    return *this;
}

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

void * EncryptedSocketInputStream::readRaw(void *data, size_t length) {
    ::read(socketfd, data, length);
    return data;
}

void * EncryptedSocketInputStream::readRaw(size_t length) {
    char * data = new char[length];
    ::read(socketfd, data, length);
    return data;
}


ssize_t read(void *buf, size_t count) {
    size_t addedToBuf = 0;
//    while (count > (outputBuffer.size() - outputBufferPos)) {
    while (addedToBuf < count) {
        if (bufferPos != 0) {
            memmove(encryptedBuffer, &encryptedBuffer[bufferPos], (CIPHER_BUFFER_LENGTH - bufferPos));
            read(socketfd, &encryptedBuffer[CIPHER_BUFFER_LENGTH - bufferPos], bufferPos);
            bufferPos = 0;
        }
        int len = outputBuffer.size() - outputBufferPos;
        if (len < count && !(count > CIPHER_BUFFER_LENGTH)) {
            int lastPos;
            if (outputBufferPos != 0) {
                memmove(outputBuffer.data(), &outputBuffer[outputBufferPos], len);
                outputBufferPos = 0;
                lastPos = len;
            }
            else {
                lastPos = outputBuffer.size();
            }
            outputBuffer.resize(count);
            
        }
        if (outputBufferPos == 0) {
            int written = std::min(outputBuffer.size(), count);
            memcpy(buf, outputBuffer.data(), written);
            addedToBuf += written;
            outputBufferPos += written;
        }
        memmove(outputBuffer, &outputBuffer[outputBufferPos], outputBuffer.size() - outputBufferPos);
        outputBufferPos = 0;
        
    }
    memcpy(buf, &outputBuffer[outputBufferPos], count);
    outputBufferPos += count;
    return;

}

}
}

