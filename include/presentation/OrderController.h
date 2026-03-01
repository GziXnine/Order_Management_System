#ifndef ORDER_CONTROLLER_H
#define ORDER_CONTROLLER_H

#include "../application/CreateOrderService.h"
#include "../application/CancelOrderService.h"
#include "../application/Interface/IOrderRepository.h"
#include "../application/Interface/IProductRepository.h"
#include "../application/Interface/ICustomerRepository.h"
#include "../domain/Inventory.h"

#include <string>

class OrderController
{
private:
  CreateOrderService &createService;
  CancelOrderService &cancelService;
  IOrderRepository &orderRepo;
  IProductRepository &productRepo;
  ICustomerRepository &customerRepo;
  Inventory &inventory;

  // UI helpers
  void printHeader(const std::string &title) const;
  void printSuccess(const std::string &msg) const;
  void printError(const std::string &msg) const;
  void printDivider() const;

  // Dynamic tables
  void showCustomerTable(int customerId) const;
  void showProductCatalog() const;
  void showOrderTable(int orderId) const;
  void showAllOrders() const;

  // Actions
  void placeOrder();
  void cancelOrder();
  void showOrder();

  std::string statusToString(EOrderStatus status) const;

public:
  OrderController(CreateOrderService &createSvc, CancelOrderService &cancelSvc, IOrderRepository &orderRepo, IProductRepository &productRepo, ICustomerRepository &customerRepo, Inventory &inventory);

  void showMenu();
};

#endif // ORDER_CONTROLLER_H
