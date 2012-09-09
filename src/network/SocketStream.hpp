
#ifndef SOCKETSTREAM_HPP
#define SOCKETSTREAM_HPP

#include "SocketInputStream.hpp"
#include "SocketOutputStream.hpp"

namespace MCServer {
namespace Network {

class SocketStream : public virtual SocketInputStream, public virtual SocketOutputStream {

};

}
}

#endif
