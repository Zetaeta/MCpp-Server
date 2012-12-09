
#ifndef BLOCKTYPES_HPP
#define BLOCKTYPES_HPP

namespace MCServer {

struct BlockType {
    std::string name;
    float hardness;
    
};

BlockType blockTypes[256] = {
    {"Air", 0},
    {"Stone", 1.5},
    {"Grass", 0.6},
    {"Dirt", 0.5},
    {},
    {},
    {},
    {"Bedrock", -1}, // Infinity.
    {"Water", 100}, // Yes, water does apparently have a finite hardness.
    // TODO: Add everything else. I am too lazy at the moment.
};

};

#endif
