#include "../../include/domain/Payment.h"

Payment::Payment() : paymentId(0), orderId(0), amount(0.0), status(EPaymentStatus::Pending) {}

Payment::Payment(int paymentId, int orderId, double amount, EPaymentStatus status)
    : paymentId(paymentId), orderId(orderId), amount(amount), status(status) {}

int Payment::getPaymentId() const { return paymentId; }
int Payment::getOrderId() const { return orderId; }
double Payment::getAmount() const { return amount; }
EPaymentStatus Payment::getStatus() const { return status; }

void Payment::setAmount(double newAmount)
{
  if (newAmount >= 0.0)
    amount = newAmount;
}

void Payment::markComplete()
{
  status = EPaymentStatus::Completed;
}

void Payment::markFailed()
{
  status = EPaymentStatus::Failed;
}

void Payment::refund()
{
  status = EPaymentStatus::Refunded;
}
