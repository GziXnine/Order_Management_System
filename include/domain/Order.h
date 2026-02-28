#ifndef ORDER_H
#define ORDER_H

#include <vector>
#include <ctime>
#include "OrderItem.h"
#include "EOrderStatus.h"

class Order
{
private:
  int orderId;
  std::vector<OrderItem> items;
  double totalAmount;
  EOrderStatus status;
  std::time_t timestamp;

public:
  Order(int orderId);
  ~Order();

  int getOrderId() const;
  const std::vector<OrderItem> &getItems() const;
  double getTotalAmount() const;
  std::time_t getTimestamp() const;
  EOrderStatus getStatus() const;

  void addItem(const OrderItem &item);
  void removeItem(const OrderItem &item);
  void updateTotalAmount();
  bool changeStatus(EOrderStatus newStatus);

  bool canBeCancelled() const;
  void cancel();
};

#endif // ORDER_H