
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
#ifdef UPDATED_ESIS_READ
    :socketfd(socketfd), decryptor(decryptor), input(CIPHER_BUFFER_LENGTH), output(CIPHER_BUFFER_LENGTH) {
#else
    :socketfd(socketfd), decryptor(decryptor), bufferPos(-1), bufferFullLength(0), outputBufferPos(0), outputBufferFullLength(0) {
    outputBuffer.resize(CIPHER_BUFFER_LENGTH + decryptor->cipher->block_size);
#endif
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


/*
 * FIXME: This method is bad and I should feel bad.
 */
ssize_t EncryptedSocketInputStream::read(void *buf, size_t count) {
#ifdef UPDATED_ESIS_READ
    cout << "read(): count = " << count << '\n'
        << "output.available() = " << output.available() << ", input.available() = " << input.available() << '\n';
    size_t added = 0;
    while (added < count) { // Buf is not full
        if (output.available() < (count - added)) {
            if (output.spaceBefore() > output.spaceAfter()) {
                output.shiftToStart();
            }
            if (input.available() == 0) { // Input buffer is empty.
                populateInput();
            }
            cout << "input.available() = " << input.available() << '\n';
            int outLen, inLen = min(input.available(), output.spaceAfter() - decryptor->cipher->block_size);
            EVP_DecryptUpdate(decryptor, output.end(), &outLen, input.begin(), inLen);
            cout << "inLen = " << inLen << ", outLen = " << outLen << '\n';
            cout << std::hex << "*output.end() = 0x" << uint16_t(*output.end()) << ", *output.begin() = 0x" << uint16_t(*output.begin()) << '\n';
            input.take(inLen);
            output.add(outLen);
            cout << std::hex << "*output.end() - 1 = 0x" << uint16_t(*(output.end() - 1)) << ", *output.begin() = 0x" << uint16_t(*output.begin()) << '\n';
            cout << "output[0] = 0x" << uint16_t(output[0]) << ", output[1] = 0x" << uint16_t(output[1]) << '\n';
        }
        int copy = min(count - added, output.available());
        cout << "copy = " << copy << '\n';
        memcpy(buf, output.begin(), copy);
        cout << "buf[0] = " << uint16_t(static_cast<uint8_t *>(buf)[0]) << '\n';
        output.take(copy);
        added += copy;
    }
    return added;
#else
    size_t addedToBuf = 0;
    Logging::Logger &log = MinecraftServer::getServer().getLogger();
    bool needMoreBytes = false; // To avoid unnecessary looping while waiting for reading.
    while (addedToBuf < count) {
        if (bufferPos > 0) { // There is space in the buffer.
            memmove(encryptedBuffer, &encryptedBuffer[bufferPos], bufferFullLength);
            bufferPos = 0;
        }
        else if (bufferPos < 0) {
            bufferPos = 0;
        }
        if (bufferFullLength < (CIPHER_BUFFER_LENGTH - 1)) {
            fd_set sockSet;
            FD_ZERO(&sockSet);
            FD_SET(socketfd, &sockSet);
            struct timeval emptyTime;
            memset(&emptyTime, 0, sizeof(emptyTime));
            int selected = select(socketfd + 1, &sockSet, NULL, NULL, &emptyTime);
            if ((selected == 1 && FD_ISSET(socketfd, &sockSet)) || needMoreBytes) {
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
            }
        }
        if ((outputBufferFullLength * 2) < count && bufferFullLength > 0) { // If the outputBuffer is less than half empty.
            if (outputBufferPos > 0) {
                memmove(outputBuffer.data(), &outputBuffer[outputBufferPos], outputBufferFullLength);
            }
            outputBufferPos = 0;
            int outLen;
            int inLen = min(size_t(bufferFullLength), (outputBuffer.size() - outputBufferFullLength) - decryptor->cipher->block_size); // Length to read.
            EVP_DecryptUpdate(decryptor, &outputBuffer[outputBufferFullLength], &outLen, &encryptedBuffer[bufferPos], inLen);
            bufferPos += inLen; // increment bufferPos by the number of bytes taken and decrypted.
            bufferFullLength -= outLen;
            outputBufferFullLength += outLen;
        }
        int toWrite = min(outputBufferFullLength, count);
        memcpy(buf, &outputBuffer[outputBufferPos], toWrite); // Move bytes from outputBuffer to output.
        addedToBuf += toWrite;
        outputBufferPos += toWrite;
        outputBufferFullLength -= toWrite;
        if (!outputBufferFullLength && !bufferFullLength) {
            needMoreBytes = true;
        }
    }
    return 0;
#endif

}

void EncryptedSocketInputStream::populateInput() {
    cout << "populateInput(): input.available() = " << input.available() << '\n';
    input.shiftToStart();
    cout << "input.spaceAfter(): " << input.spaceAfter() << '\n';
    cout << "input.spaceBefore(): " << input.spaceBefore() << '\n';
    cout << "input.fullSize(): " << input.fullSize() << '\n';
    ssize_t bytesRead = ::read(socketfd, input.end(), input.spaceAfter());
    cout << "Read " << bytesRead << " bytes!\n";
    if (bytesRead < 0) {
        std::cerr << "bytesRead < 0!\n";
        return;
    }
    input.add(bytesRead);
}

}
}

