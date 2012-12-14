
#include "ItemStack.hpp"

namespace MCServer {

ItemStack::ItemStack(uint16_t id, uint8_t size)
:id(id), size_(size) {}

ItemStack & ItemStack::operator--() {
    --size_;
    return *this;
}

ItemStack ItemStack::operator--(int) {
    ItemStack other(*this);
    --size_;
    return other;
}

ItemStack & ItemStack::operator++() {
    ++size_;
    return *this;
}

ItemStack ItemStack::operator++(int) {
    ItemStack other(*this);
    ++size_;
    return other;
}

uint8_t ItemStack::size() const {
    return size_;
}

uint16_t ItemStack::getId() const {
    return id;
}

void ItemStack::setSize(uint8_t sz) {
    size_ = sz;
}

void ItemStack::setId(uint16_t id_) {
    id = id_;
}

}
