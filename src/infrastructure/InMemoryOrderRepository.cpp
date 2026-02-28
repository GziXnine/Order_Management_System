#include "../../include/infrastructure/InMemoryOrderRepository.h"
#include <stdexcept>

void InMemoryOrderRepository::save(const Order &order)
{
  orders[order.getOrderId()] = order;
}

Order InMemoryOrderRepository::findById(int orderId)
{
  auto it = orders.find(orderId);
  if (it == orders.end())
    throw std::runtime_error("Order not found with id: " + std::to_string(orderId));

  return it->second;
}

std::vector<Order> InMemoryOrderRepository::getAll() const
{
  std::vector<Order> result;
  for (const auto &pair : orders)
    result.push_back(pair.second);

  return result;
}

int InMemoryOrderRepository::nextId()
{
  return nextOrderId++;
}
