#ifndef ORDER_ITEM_H
#define ORDER_ITEM_H

#include "Product.h"

// Product class is included here because OrderItem has a Product as a member. This is composition — an OrderItem "has a" Product. We need the full definition of Product to declare a Product member variable. If we only had a pointer or reference to Product, we could get away with a forward declaration (e.g., "class Product;") and avoid including the header, but in this case, we need the complete class definition.

class OrderItem
{
private:
  Product *product;
  int quantity;
  double lineTotal;

public:
  OrderItem();
  OrderItem(const Product &product, int quantity);

  const Product &getProduct() const;
  int getQuantity() const;
  double getLineTotal() const;

  void setProduct(const Product &product);
  void setQuantity(int quantity);

  void calculateLineTotal();
};

#endif // ORDER_ITEM_H