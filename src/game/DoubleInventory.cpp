
#include <stdint.h>

#include "DoubleInventory.hpp"

namespace MCServer {

ItemStack * DoubleInventory::begin() {
    return items;
}

ItemStack * DoubleInventory::end() {
    return items + 54;
}

const ItemStack * DoubleInventory::begin() const {
    return items;
}

const ItemStack * DoubleInventory::end() const {
    return items + 54;
}

ItemStack & DoubleInventory::operator[](int index) {
    return items[index];
}

const ItemStack & DoubleInventory::operator[](int index) const {
    return items[index];
}

}
