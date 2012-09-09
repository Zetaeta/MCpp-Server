
#include "EncryptedSocketStream.hpp"

namespace MCServer {
namespace Network {

EncryptedSocketStream::EncryptedSocketStream(int socketfd, EVP_CIPHER_CTX *encryptor, EVP_CIPHER_CTX *decryptor)
    :EncryptedSocketInputStream(socketfd, decryptor), EncryptedSocketOutputStream(socketfd, encryptor) {

}

}
}
