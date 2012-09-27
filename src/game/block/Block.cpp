
#include "Block.hpp"

namespace MCServer {

Block::Block(uint16_t id, uint8_t metadata)
:id(id), metadata(metadata) {
    
}

Block::~Block() {

}

uint16_t Block::getId() {
    return id;
}

uint8_t Block::getMetadata() {
    return metadata;
}

}

