#ifndef FAKE_SHIPPING_SERVICE_H
#define FAKE_SHIPPING_SERVICE_H

#include <string>
#include "../application/Interface/IShippingService.h"

class FakeShippingService : public IShippingService
{
private:
  int nextShipmentId;

public:
  FakeShippingService();

  void createShipment(const Order &order) override;
  std::string generateTrackingNumber() const;
};

#endif // FAKE_SHIPPING_SERVICE_H
