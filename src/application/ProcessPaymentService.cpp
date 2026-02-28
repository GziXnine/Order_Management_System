#include "../../include/application/ProcessPaymentService.h"
#include <stdexcept>

Payment ProcessPaymentService::process(const Order &order)
{
  bool success = paymentProvider.charge(order.getTotalAmount());

  Payment payment(paymentProvider.generatePaymentId(), order.getOrderId(), order.getTotalAmount());

  if (success)
    payment.markComplete();
  else
    payment.markFailed();

  return payment;
}