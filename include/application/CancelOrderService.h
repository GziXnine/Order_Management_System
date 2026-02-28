#ifndef CANCEL_ORDER_SERVICE_H
#define CANCEL_ORDER_SERVICE_H

#include "../domain/Inventory.h"
#include "../domain/Order.h"
#include "Interface/IOrderRepository.h"
#include "Interface/IPaymentProvider.h"
#include "Interface/INotificationService.h"

class CancelOrderService
{
private:
  IOrderRepository &orderRepo;
  Inventory &inventory;
  IPaymentProvider &paymentProvider;
  INotificationService &notificationService;

public:
  CancelOrderService(IOrderRepository &repo, Inventory &inv, IPaymentProvider &payment, INotificationService &notifier);

  void cancelOrder(int orderId);
};

#endif // CANCEL_ORDER_SERVICE_H