
#ifndef PACKETCHATMESSAGE_HPP
#define PACKETCHATMESSAGE_HPP

#include <Util/stlfwd.hpp>

namespace MCServer {

class PacketChatMessage : public GenericPacket {
    void read(InputStream &stream) {
        readHelper(stream, name);
    }
    
private:
    std::string name;
};

}

#endif
