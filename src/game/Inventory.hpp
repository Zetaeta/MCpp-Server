
#ifndef INVENTORY_HPP
#define INVENTORY_HPP

namespace MCServer {

class ItemStack;

class Inventory {
public:
    virtual ItemStack * begin() = 0;
    virtual ItemStack * end() = 0;

    virtual const ItemStack * begin() const = 0;
    virtual const ItemStack * end() const = 0;

    virtual ItemStack & operator[](int) = 0;
    virtual const ItemStack & operator[](int) const = 0;
    virtual ~Inventory() {}
};

}

#endif
