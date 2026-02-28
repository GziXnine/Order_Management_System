#include "../../include/domain/Order.h"
#include <algorithm>
#include <stdexcept>

Order::Order() : orderId(0), customerId(0), items(), totalAmount(0.0), status(EOrderStatus::Placed)
{
  timestamp = std::time(nullptr);
}

Order::Order(int orderId, int customerId) : orderId(orderId), customerId(customerId), items(), totalAmount(0.0), status(EOrderStatus::Placed)
{
  if (orderId <= 0)
    throw std::invalid_argument("orderId must be positive");
  if (customerId <= 0)
    throw std::invalid_argument("customerId must be positive");

  timestamp = std::time(nullptr);
}

Order::~Order() = default;

int Order::getOrderId() const { return orderId; }
int Order::getCustomerId() const { return customerId; }
const std::vector<OrderItem> &Order::getItems() const { return items; }
double Order::getTotalAmount() const { return totalAmount; }
std::time_t Order::getTimestamp() const { return timestamp; }
EOrderStatus Order::getStatus() const { return status; }

void Order::addItem(const OrderItem &item)
{
  if (item.getQuantity() <= 0)
    throw std::invalid_argument("Item quantity must be at least 1");

  items.push_back(item);
  updateTotalAmount();
}

void Order::removeItem(const OrderItem &item)
{
  auto it = std::find(items.begin(), items.end(), item);
  if (it != items.end())
  {
    items.erase(it);
    updateTotalAmount();
  }
}

void Order::updateTotalAmount()
{
  totalAmount = 0.0;
  for (const auto &item : items)
    totalAmount += item.getLineTotal();
}

bool Order::changeStatus(EOrderStatus newStatus)
{
  if (status == EOrderStatus::Delivered || status == EOrderStatus::Cancelled || status == EOrderStatus::Rejected || status == EOrderStatus::PaymentFailed)
    return false;

  status = newStatus;
  return true;
}

bool Order::canBeCancelled() const
{
  return status == EOrderStatus::Placed || status == EOrderStatus::Validated || status == EOrderStatus::InventoryReserved;
}

void Order::cancel()
{
  if (canBeCancelled())
    status = EOrderStatus::Cancelled;
}