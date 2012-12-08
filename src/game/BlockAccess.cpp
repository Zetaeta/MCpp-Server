
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
    return BlockAccess<2>(chunk, x, z);
}

const BlockAccess<2> BlockAccess<1>::operator[](int z) const {
    return BlockAccess<2>(chunk, x, z);
}

BlockAccess<2>::BlockAccess(Chunk &chunk, int x, int z)
:chunk(chunk), x(x) {
}

BlockAccess<3> BlockAccess<2>::operator[](int y) {
    return BlockAccess<2>(chunk, x, z);
}

const BlockAccess<2> BlockAccess<1>::operator[](int z) const {
    return BlockAccess<2>(chunk, x, z);
}


}
