
#ifndef INVENTORY_HPP
#define INVENTORY_HPP

namespace MCServer {

class ItemStack;

class Inventory {
public:
    ItemStack * begin();
    ItemStack * end();
};

}

#endif
