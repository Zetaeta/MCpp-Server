
#include "SocketStream.hpp"

namespace MCServer {
namespace Network {

SocketStream::SocketStream(int socketfd)
    :SocketInputStream(socketfd), SocketOutputStream(socketfd) {

}

}
}
