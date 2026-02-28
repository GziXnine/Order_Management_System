#include "../../include/application/CreateOrderService.h"
#include <stdexcept>

CreateOrderService::CreateOrderService(IOrderRepository &repo, Inventory &inv, IProductRepository &productRepository,
                                       ICustomerRepository &customerRepository, IPaymentProvider &payment,
                                       IShippingService &shipping, INotificationService &notifier)
    : orderRepo(repo), inventory(inv), productRepo(productRepository), customerRepo(customerRepository),
      paymentProvider(payment), shippingService(shipping), notificationService(notifier) {}

Order CreateOrderService::createOrder(int customerId, const std::vector<std::pair<int, int>> &items)
{
  if (items.empty())
    throw std::invalid_argument("Order must contain items");

  // Validate customer exists
  Customer customer = customerRepo.findById(customerId);

  // Generate auto-increment order ID
  int orderId = orderRepo.nextId();
  Order order(orderId, customerId);

  // Reserve Inventory and fetch product details
  for (const auto &item : items)
  {
    int productId = item.first;
    int quantity = item.second;

    Product product = productRepo.findById(productId);
    if (!product.isActive())
      throw std::runtime_error("Product '" + product.getName() + "' is inactive");

    if (!inventory.isAvailable(productId, quantity))
      throw std::runtime_error("Product '" + product.getName() + "' is out of stock");

    inventory.reserve(productId, quantity);
    order.addItem(OrderItem(product, quantity));
  }

  order.updateTotalAmount();

  // Check customer balance
  if (!customer.canAfford(order.getTotalAmount()))
  {
    // rollback reservation
    for (const auto &item : items)
      inventory.release(item.first, item.second);
    throw std::runtime_error("Insufficient customer balance");
  }

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

  // Deduct customer balance
  customer.deductBalance(order.getTotalAmount());
  customerRepo.save(customer);

  // Create shipment
  shippingService.createShipment(order);

  order.changeStatus(EOrderStatus::PaymentProcessed);

  orderRepo.save(order);

  notificationService.notify("Order #" + std::to_string(orderId) + " created successfully for " + customer.getName());

  return order;
}