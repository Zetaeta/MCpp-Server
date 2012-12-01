
#include "ItemStack.hpp"

namespace MCServer {

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

uint16_t getId() const {
    return id;
}

void setSize(uint8_t sz) {
    size_ = sz;
}

void setId(uint16_t id_) {
    id = id_;
}

}
