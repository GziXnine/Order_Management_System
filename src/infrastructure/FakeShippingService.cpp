#include "../../include/infrastructure/FakeShippingService.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>

FakeShippingService::FakeShippingService() : nextShipmentId(5000)
{
  std::srand(static_cast<unsigned>(std::time(nullptr)));
}

void FakeShippingService::createShipment(const Order &order)
{
  std::string tracking = generateTrackingNumber();
  std::cout << "[Shipping] Shipment #" << nextShipmentId
            << " created for Order #" << order.getOrderId()
            << " | Tracking: " << tracking << "\n";
  nextShipmentId++;
}

std::string FakeShippingService::generateTrackingNumber() const
{
  std::ostringstream oss;
  oss << "TRK-" << (std::rand() % 900000 + 100000);
  return oss.str();
}
