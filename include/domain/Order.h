#ifndef ORDER_H
#define ORDER_H

#include <vector>
#include <ctime>
#include "OrderItem.h"
#include "enum/EOrderStatus.h"

class Order
{
private:
  int orderId;
  int customerId;
  std::vector<OrderItem> items;
  double totalAmount;
  EOrderStatus status;
  std::time_t timestamp;

public:
  Order();
  Order(int orderId, int customerId);
  ~Order();

  int getOrderId() const;
  int getCustomerId() const;
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