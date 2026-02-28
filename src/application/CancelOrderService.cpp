#include "../../include/application/CancelOrderService.h"
#include <stdexcept>

CancelOrderService::CancelOrderService(IOrderRepository &repo, Inventory &inv, IPaymentProvider &payment, INotificationService &notifier)
    : orderRepo(repo), inventory(inv), paymentProvider(payment), notificationService(notifier) {}

void CancelOrderService::cancelOrder(int orderId)
{
  Order order = orderRepo.findById(orderId);

  if (!order.canBeCancelled())
    throw std::runtime_error("Order cannot be cancelled");

  for (const auto &item : order.getItems())
    inventory.release(item.getProduct().getId(), item.getQuantity());

  paymentProvider.refund(order.getTotalAmount());

  order.changeStatus(EOrderStatus::Cancelled);

  orderRepo.save(order);

  notificationService.notify("Order cancelled successfully");
}