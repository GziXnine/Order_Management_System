#ifndef CREATE_ORDER_SERVICE_H
#define CREATE_ORDER_SERVICE_H

#include <vector>
#include "../domain/Order.h"
#include "../domain/Inventory.h"
#include "../domain/Product.h"
#include "../domain/Customer.h"
#include "Interface/IOrderRepository.h"
#include "Interface/IPaymentProvider.h"
#include "Interface/IShippingService.h"
#include "Interface/INotificationService.h"
#include "Interface/IProductRepository.h"
#include "Interface/ICustomerRepository.h"

class CreateOrderService
{
private:
  IOrderRepository &orderRepo;
  Inventory &inventory;
  IPaymentProvider &paymentProvider;
  IShippingService &shippingService;
  INotificationService &notificationService;
  IProductRepository &productRepo;
  ICustomerRepository &customerRepo;

public:
  CreateOrderService(IOrderRepository &repo, Inventory &inv, IProductRepository &productRepository,
                     ICustomerRepository &customerRepository, IPaymentProvider &payment,
                     IShippingService &shipping, INotificationService &notifier);

  Order createOrder(int customerId, const std::vector<std::pair<int, int>> &items);
};

#endif