
#ifndef INVENTORYHOLDER_HPP
#define INVENTORYHOLDER_HPP

#include "Inventory.hpp"

namespace MCServer {

class InventoryHolder {
public:
    InventoryHolder(Inventory *inventory);
    Inventory & getInventory();
    const Inventory & getInventory() const;
protected:
    Inventory *inventory;
};

}

#endif
