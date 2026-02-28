#include "../../include/domain/Shipment.h"

Shipment::Shipment() : shipmentId(0), orderId(0), address(""), trackingNumber(""), status(EShipmentStatus::Pending) {}
Shipment::Shipment(int shipmentId, int orderId, const std::string &address)
    : shipmentId(shipmentId), orderId(orderId), address(address), trackingNumber(""), status(EShipmentStatus::Pending) {}

int Shipment::getShipmentId() const { return shipmentId; }
int Shipment::getOrderId() const { return orderId; }
const std::string &Shipment::getAddress() const { return address; }
const std::string &Shipment::getTrackingNumber() const { return trackingNumber; }
EShipmentStatus Shipment::getStatus() const { return status; }

void Shipment::setAddress(const std::string &address)
{
  this->address = address;
}

void Shipment::setTrackingNumber(const std::string &trackingNumber)
{
  this->trackingNumber = trackingNumber;
}

void Shipment::markShipped()
{
  status = EShipmentStatus::Shipped;
}

void Shipment::markDelivered()
{
  status = EShipmentStatus::Delivered;
}