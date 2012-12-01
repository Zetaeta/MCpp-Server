
#ifndef PLAYERINVENTORY_HPP
#define PLAYERINVENTORY_HPP

#include "Inventory.hpp"
#include "ItemStack.hpp"

namespace MCServer {

class PlayerInventory : public Inventory {
public:
    ItemStack * begin();
    ItemStack * end();
    const ItemStack * begin() const;
    const ItemStack * end() const;

    ItemStack & operator[](int);
    const ItemStack & operator[](int) const;
private:
   ItemStack items[44]; 
};

}

#endif
