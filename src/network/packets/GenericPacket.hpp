
#ifndef GENERICPACKET_HPP
#define GENERICPACKET_HPP

#include <IOStream/InputStream.hpp>

namespace MCServer {

class GenericPacket {
public:
    virtual void read(IOStream::InputStream &stream) = 0;
    virtual void write(IOStream::InputStream &stream) = 0;
};

}

#include "GenericPacket.tcc"

#endif

