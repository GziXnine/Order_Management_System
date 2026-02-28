#ifndef IN_MEMORY_ORDER_REPOSITORY_H
#define IN_MEMORY_ORDER_REPOSITORY_H

#include <map>
#include "../application/Interface/IOrderRepository.h"

class InMemoryOrderRepository : public IOrderRepository
{
private:
  std::map<int, Order> orders;
  int nextOrderId = 1;

public:
  void save(const Order &order) override;
  Order findById(int orderId) override;
  std::vector<Order> getAll() const override;
  int nextId() override;
};

#endif // IN_MEMORY_ORDER_REPOSITORY_H
