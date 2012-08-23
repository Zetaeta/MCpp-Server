
#ifndef SOCKETSTREAM_HPP
#define SOCKETSTREAM_HPP

#include "SocketInputStream.hpp"
#include "SocketOutputStream.hpp"

namespace MCServer {
namespace Network {

class SocketStream : public SocketInputStream, public SocketOutputStream {
public:
    SocketStream(int sockfd);
};

}
}

#endif
