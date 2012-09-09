
#include "PlainSocketStream.hpp"

namespace MCServer {
namespace Network {

PlainSocketStream::PlainSocketStream(int socketfd)
    :PlainSocketInputStream(socketfd), PlainSocketOutputStream(socketfd) {

}

}
}
