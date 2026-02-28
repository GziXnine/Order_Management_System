#ifndef ORDER_ITEM_H
#define ORDER_ITEM_H

#include "Product.h"

// Aggregation: OrderItem "has a" Product, but OrderItem can exist without a Product (e.g., if we want to create an OrderItem and set the Product later). Using a pointer allows for this flexibility.

class OrderItem
{
private:
  Product *product;
  int quantity;
  double lineTotal;

public:
  OrderItem();
  OrderItem(const Product &product, int quantity);
  ~OrderItem();

  const Product &getProduct() const;
  int getQuantity() const;
  double getLineTotal() const;

  void setProduct(const Product &product);
  void setQuantity(int quantity);

  void calculateLineTotal();

  bool operator==(const OrderItem &other) const;
};

#endif // ORDER_ITEM_H