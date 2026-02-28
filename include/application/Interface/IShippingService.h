#ifndef ISHIPPING_SERVICE_H
#define ISHIPPING_SERVICE_H

#include "../../domain/Order.h"

class IShippingService
{
public:
  virtual void createShipment(const Order &order) = 0;

  virtual ~IShippingService() = default;
};

#endif