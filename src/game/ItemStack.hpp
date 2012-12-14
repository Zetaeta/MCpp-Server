
#ifndef ITEMSTACK_HPP
#define ITEMSTACK_HPP

#include <stdint.h>

namespace MCServer {

class ItemStack {
public:
    ItemStack() = default;

    ItemStack(uint16_t id, uint8_t size);

    ItemStack & operator--();
    ItemStack operator--(int);
    ItemStack & operator++();
    ItemStack operator++(int);

    uint8_t size() const;
    uint16_t getId() const;

    void setSize(uint8_t);
    void setId(uint16_t);
private:
    uint16_t id;
    uint8_t size_;
};

}

#endif
