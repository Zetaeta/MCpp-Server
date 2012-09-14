
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

namespace MCServer {
namespace Network {

EncryptedSocketInputStream::EncryptedSocketInputStream(int socketfd, EVP_CIPHER_CTX *decryptor)
    :socketfd(socketfd), decryptor(decryptor), outputBufferPos(0), bufferPos(-1), bufferFullLength(0), outputBufferFullLength(0) {
    outputBuffer.resize(CIPHER_BUFFER_LENGTH + decryptor->cipher->block_size);
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


/*
 * FIXME: This method is bad and I should feel bad.
 */
ssize_t EncryptedSocketInputStream::read(void *buf, size_t count) {
    size_t addedToBuf = 0;
//    while (count > (outputBuffer.size() - outputBufferPos)) {
    Logging::Logger &log = MinecraftServer::getServer().getLogger();
//    log << "Characters in encrypted buffer: " << std::string(reinterpret_cast<char *>(&encryptedBuffer[bufferPos]), bufferFullLength) << '\n';
//    log << "Characters in output buffer: " << std::string(reinterpret_cast<char *>(&outputBuffer[outputBufferPos]), outputBufferFullLength) << '\n';
    bool needMoreBytes = false; // To avoid unnecessary looping while waiting for reading.
    while (addedToBuf < count) {
//        log << "Characters in encrypted buffer before reading: 0x" << std::hex << uint16_t(encryptedBuffer[0]) << '\n';
        if (bufferPos > 0) { // There is space in the buffer.
            memmove(encryptedBuffer, &encryptedBuffer[bufferPos], bufferFullLength);
            bufferPos = 0;
        }
        else if (bufferPos < 0) {
            bufferPos = 0;
        }
        if (bufferFullLength < (CIPHER_BUFFER_LENGTH - 1)) {
//            log << "Space in input buffer for reading!\n";
            fd_set sockSet;
            FD_ZERO(&sockSet);
            FD_SET(socketfd, &sockSet);
            struct timeval emptyTime;
            memset(&emptyTime, 0, sizeof(emptyTime));
            int selected = select(socketfd + 1, &sockSet, NULL, NULL, &emptyTime);
            if ((selected == 1 && FD_ISSET(socketfd, &sockSet)) || needMoreBytes) {
//                log << "Going to read!\n";
                int read = ::read(socketfd, &encryptedBuffer[bufferFullLength], CIPHER_BUFFER_LENGTH - bufferFullLength - 1);
                if (read < 0) {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        read = 0;
                    }
                    else {
                        log << WARNING << "read < 0! Strerror = " << strerror(read) << '\n';
                        read = 0;
                    }
                }
            
                bufferFullLength += read;
                log << "Read " << std::dec << read << " bytes from stream!\n";
            }
        }
        if (bufferFullLength) {
            log << "Characters in encrypted buffer: " << std::hex;
            for (int i=bufferPos; i<(bufferPos + bufferFullLength); ++i) {
               log << "0x" << uint16_t(encryptedBuffer[i]) << ' ';
            }
            log << '\n';
        }
        if ((outputBufferFullLength * 2) < count && bufferFullLength > 0) { // If the outputBuffer is less than half empty.
            log << "Decrypting data to outputBuffer!\n";
            if (outputBufferPos > 0) {
                memmove(outputBuffer.data(), &outputBuffer[outputBufferPos], outputBufferFullLength);
            }
            outputBufferPos = 0;
            int outLen;
            int inLen = min(size_t(bufferFullLength), (outputBuffer.size() - outputBufferFullLength) - decryptor->cipher->block_size); // Length to read.
            EVP_DecryptUpdate(decryptor, &outputBuffer[outputBufferFullLength], &outLen, &encryptedBuffer[bufferPos], inLen);
            log << "Decrypted " << inLen << " bytes (" << std::hex;
            for (int i=0; i<inLen; ++i) {
                log << "0x" << static_cast<uint16_t>(encryptedBuffer[bufferPos + i]) << ' ';
            }
            log << ") to " << outLen << " bytes (";
            for (int i=0; i<outLen; ++i) {
                log << "0x" << static_cast<uint16_t>(outputBuffer[outputBufferFullLength + i]) << ' ';
            }
            log << ")\n";
            bufferPos += inLen; // increment bufferPos by the number of bytes taken and decrypted.
            bufferFullLength -= outLen;
            outputBufferFullLength += outLen;
        }
        int toWrite = min(outputBufferFullLength, count);
        log << INFO << "toWrite: " << toWrite << '\n';
        memcpy(buf, &outputBuffer[outputBufferPos], toWrite); // Move bytes from outputBuffer to output.
        if (outputBufferFullLength)
            log << "outputBuffer[outputBufferPos] = 0x" << int(outputBuffer[outputBufferPos]) << ", *buf = 0x" << int(*static_cast<uint8_t *>(buf)) << '\n' << std::dec;
        addedToBuf += toWrite;
        outputBufferPos += toWrite;
        outputBufferFullLength -= toWrite;
        if (!outputBufferFullLength && !bufferFullLength) {
//            log << "Both buffers empty, need more bytes!\n";
            needMoreBytes = true;
        }
    }
    return 0;

}

}
}

