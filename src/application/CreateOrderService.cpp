#include "../../include/application/CreateOrderService.h"
#include <stdexcept>

CreateOrderService::CreateOrderService(IOrderRepository &repo, Inventory &inv, IProductRepository &productRepository, IPaymentProvider &payment, IShippingService &shipping, INotificationService &notifier)
    : orderRepo(repo), inventory(inv), paymentProvider(payment), shippingService(shipping), notificationService(notifier), productRepo(productRepository) {}

Order CreateOrderService::createOrder(int customerId, const std::vector<std::pair<int, int>> &items)
{
  if (items.empty())
    throw std::invalid_argument("Order must contain items");

  Order order(customerId);

  // Reserve Inventory and fetch product details
  for (const auto &item : items)
  {
    int productId = item.first;
    int quantity = item.second;

    Product product = productRepo.findById(productId);
    if (!product.isActive())
      throw std::runtime_error("Product is inactive");

    if (!inventory.isAvailable(productId, quantity))
      throw std::runtime_error("Product not available");

    inventory.reserve(productId, quantity);
    order.addItem(OrderItem(product, quantity));
  }

  order.updateTotalAmount();

  // Process Payment
  bool paymentSuccess = paymentProvider.charge(order.getTotalAmount());

  if (!paymentSuccess)
  {
    // rollback reservation
    for (const auto &item : items)
      inventory.release(item.first, item.second);

    order.changeStatus(EOrderStatus::PaymentFailed);
    orderRepo.save(order);

    throw std::runtime_error("Payment failed");
  }

  // Deduct stock permanently
  for (const auto &item : items)
    inventory.deduct(item.first, item.second);

  // Create shipment
  shippingService.createShipment(order);

  order.changeStatus(EOrderStatus::PaymentProcessed);

  orderRepo.save(order);

  notificationService.notify("Order created successfully");

  return order;
}