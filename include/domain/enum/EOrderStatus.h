#ifndef E_ORDER_STATUS_H
#define E_ORDER_STATUS_H

enum class EOrderStatus
{
  Placed,
  Validated,
  InventoryReserved,
  PaymentProcessed,
  Shipped,
  Delivered,
  Cancelled,
  Rejected,
  PaymentFailed,
};

#endif // E_ORDER_STATUS_H