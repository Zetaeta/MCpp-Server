
#include <stdint.h>

#include "SingleInventory.hpp"

namespace MCServer {

ItemStack * SingleInventory::begin() {
    return items;
}

ItemStack * SingleInventory::end() {
    return items + 27;
}

const ItemStack * SingleInventory::begin() const {
    return items;
}

const ItemStack * SingleInventory::end() const {
    return items + 27;
}

ItemStack & SingleInventory::operator[](int index) {
    return items[index];
}

const ItemStack & SingleInventory::operator[](int index) const {
    return items[index];
}

}
