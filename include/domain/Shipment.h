#ifndef SHIPMENT_H
#define SHIPMENT_H

#include <string>
#include "enum/EShipmentStatus.h"

class Shipment
{
private:
  int shipmentId;
  int orderId;
  std::string address;
  std::string trackingNumber;
  EShipmentStatus status;

public:
  Shipment();
  Shipment(int shipmentId, int orderId, const std::string &address);

  int getShipmentId() const;
  int getOrderId() const;
  const std::string &getAddress() const;
  const std::string &getTrackingNumber() const;
  EShipmentStatus getStatus() const;

  void setAddress(const std::string &address);
  void setTrackingNumber(const std::string &trackingNumber);
  void markShipped();
  void markDelivered();
};

#endif // SHIPMENT_H