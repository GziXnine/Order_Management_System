#ifndef CREATE_ORDER_SERVICE_H
#define CREATE_ORDER_SERVICE_H

#include <vector>
#include "../domain/Order.h"
#include "../domain/Inventory.h"
#include "../domain/Product.h"
#include "Interface/IOrderRepository.h"
#include "Interface/IPaymentProvider.h"
#include "Interface/IShippingService.h"
#include "Interface/INotificationService.h"
#include "Interface/IProductRepository.h"

class CreateOrderService
{
private:
  IOrderRepository &orderRepo;
  Inventory &inventory;              // Business Logic Core.
  IPaymentProvider &paymentProvider; // Maybe Change Like (Stripe, Paypal, Fake, Cash, Bank Gateway). OCP, DIP
  IShippingService &shippingService;
  INotificationService &notificationService;
  IProductRepository &productRepo;

public:
  CreateOrderService(IOrderRepository &repo, Inventory &inv, IProductRepository &productRepository, IPaymentProvider &payment, IShippingService &shipping, INotificationService &notifier);

  Order createOrder(int customerId, const std::vector<std::pair<int, int>> &items);
};

#endif