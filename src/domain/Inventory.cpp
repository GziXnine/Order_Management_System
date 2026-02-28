#include "../../include/domain/Inventory.h"

bool Inventory::isAvailable(int productId, int qty) const
{
  if (qty <= 0)
    return false;

  auto it = products.find(productId);
  int total = (it != products.end()) ? it->second : 0;
  auto rit = reservedProducts.find(productId);
  int reserved = (rit != reservedProducts.end()) ? rit->second : 0;

  return (total - reserved) >= qty;
}

bool Inventory::reserve(int productId, int qty)
{
  if (qty <= 0)
    return false;

  if (!isAvailable(productId, qty))
    return false;

  reservedProducts[productId] += qty;
  return true;
}

// إلغاء الحجز
void Inventory::release(int productId, int qty)
{
  if (qty <= 0)
    return;

  auto it = reservedProducts.find(productId);
  if (it == reservedProducts.end())
    return;

  it->second = std::max(0, it->second - qty);
  if (it->second == 0)
    reservedProducts.erase(it);
}

// بيع / خروج منتج من المخزن
void Inventory::deduct(int productId, int qty)
{
  if (qty <= 0)
    return;

  auto it = products.find(productId);
  if (it == products.end())
    return;

  it->second -= qty;
  if (it->second < 0)
    it->second = 0;

  // reduce reserved if present
  auto rit = reservedProducts.find(productId);
  if (rit != reservedProducts.end())
  {
    rit->second = std::max(0, rit->second - qty);
    if (rit->second == 0)
      reservedProducts.erase(rit);
  }
}

void Inventory::restock(int productId, int qty)
{
  if (qty <= 0)
    return;

  products[productId] += qty;
}
