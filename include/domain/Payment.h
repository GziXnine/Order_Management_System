#ifndef PAYMENT_H
#define PAYMENT_H

#include "enum/EPaymentStatus.h"

class Payment
{
private:
  int paymentId;
  int orderId;
  double amount;
  EPaymentStatus status;

public:
  Payment();
  Payment(int paymentId, int orderId, double amount, EPaymentStatus status = EPaymentStatus::Pending);

  int getPaymentId() const;
  int getOrderId() const;
  double getAmount() const;
  EPaymentStatus getStatus() const;

  void setAmount(double amount);
  void markComplete();
  void markFailed();
  void refund();
};

#endif // PAYMENT_H