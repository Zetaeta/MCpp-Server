
#ifndef PLAINSOCKETSTREAM_HPP
#define PLAINSOCKETSTREAM_HPP

#include "PlainSocketInputStream.hpp"
#include "PlainSocketOutputStream.hpp"
#include "SocketStream.hpp"

namespace MCServer {
namespace Network {

class PlainSocketStream : public PlainSocketInputStream, public PlainSocketOutputStream, public SocketStream {
public:
    PlainSocketStream(int sockfd);
};

}
}

#endif
