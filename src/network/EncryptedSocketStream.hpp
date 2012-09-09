
#ifndef ENCRYPTEDSOCKETSTREAM_HPP
#define ENCRYPTEDSOCKETSTREAM_HPP

#include "EncryptedSocketInputStream.hpp"
#include "EncryptedSocketOutputStream.hpp"
#include "SocketStream.hpp"

namespace MCServer {
namespace Network {

class EncryptedSocketStream : public EncryptedSocketInputStream, public EncryptedSocketOutputStream, public SocketStream {
public:
    EncryptedSocketStream(int sockfd, EVP_CIPHER_CTX *encryptor, EVP_CIPHER_CTX *decryptor);
    EncryptedSocketStream() {}
};

}
}

#endif
