
#include "BlockAccess.hpp"

namespace MCServer {

// BlockAccess<1>

namespace {

const int chunkHeight = 256; // For if I change chunks to be only 16 blocks.

}

BlockAccess<1>::BlockAccess(Chunk &chunk, int x)
:chunk(chunk), x(x) {
}

BlockAccess<2> BlockAccess<1>::operator[](int z) {

}

}
