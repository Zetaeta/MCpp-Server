
#include <stdint.h>

#include "PlayerInventory.hpp"

namespace MCServer {

ItemStack * PlayerInventory::begin() {
    return items;
}

ItemStack * PlayerInventory::end() {
    return items + 44;
}

const ItemStack * PlayerInventory::begin() const {
    return items;
}

const ItemStack * PlayerInventory::end() const {
    return items + 44;
}

ItemStack & PlayerInventory::operator[](int index) {
    return items[index];
}

const ItemStack & PlayerInventory::operator[](int index) const {
    return items[index];
}

}
