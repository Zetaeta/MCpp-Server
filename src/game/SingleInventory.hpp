
#ifndef SINGLEINVENTORY_HPP
#define SINGLEINVENTORY_HPP

#include "Inventory.hpp"

namespace MCServer {

class SingleInventory {
public:
    ItemStack * begin();
    ItemStack * end();
    const ItemStack * begin() const;
    const ItemStack * end() const;

    ItemStack & operator[](int);
    const ItemStack & operator[](int) const;
private:
    ItemStack items[27]
};

}

#endif
