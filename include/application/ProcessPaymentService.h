#ifndef PROCESS_PAYMENT_SERVICE_H
#define PROCESS_PAYMENT_SERVICE_H

#include "../domain/Order.h"
#include "../domain/Payment.h"
#include "Interface/IPaymentProvider.h"

class ProcessPaymentService
{
private:
  IPaymentProvider &paymentProvider;

public:
  explicit ProcessPaymentService(IPaymentProvider &provider);

  Payment process(const Order &order);
};

#endif // PROCESS_PAYMENT_SERVICE_H