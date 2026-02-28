#ifndef IORDER_REPOSITORY_H
#define IORDER_REPOSITORY_H

#include "../domain/Order.h"

class IOrderRepository
{
public:
  virtual void save(const Order &order) = 0;
  virtual Order findById(int orderId) = 0;

  virtual ~IOrderRepository() = default;
};

#endif
