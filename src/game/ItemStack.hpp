
#ifndef ITEMSTACK_HPP
#define ITEMSTACK_HPP

namespace MCServer {

class ItemStack {
public:
    ItemStack & operator--();
    ItemStack operator--(int);
    ItemStack & operator++();
    ItemStack operator++(int);

    uint8_t size() const;
    uint16_t getId() const;

    void setSize(uint8_t);
    void setId(uint16_t);
private:
    uint8_t size_;
    uint16_t id;
};

}

#endif
