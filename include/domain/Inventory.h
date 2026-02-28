#ifndef INVENTORY_H
#define INVENTORY_H

#include <map>

class Inventory
{
private:
  std::map<int, int> products;         // map<productId, total> productId -> total quantity in stock
  std::map<int, int> reservedProducts; // map<productId, reserved> productId -> quantity reserved for orders

public:
  bool isAvailable(int productId, int qty) const;
  bool reserve(int productId, int qty);
  void release(int productId, int qty);
  void deduct(int productId, int qty);
  void restock(int productId, int qty);
};

#endif // INVENTORY_H