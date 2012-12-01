
#ifndef DOUBLEINVENTORY_HPP
#define DOUBLEINVENTORY_HPP

#include "Inventory.hpp"

namespace MCServer {

class DoubleInventory {
public:
    ItemStack * begin();
    ItemStack * end();
    const ItemStack * begin() const;
    const ItemStack * end() const;

    ItemStack & operator[](int);
    const ItemStack & operator[](int) const;
private:
    ItemStack items[54]
};

}

#endif
