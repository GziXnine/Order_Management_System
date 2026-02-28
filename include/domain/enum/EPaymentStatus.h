#ifndef E_PAYMENT_STATUS_H
#define E_PAYMENT_STATUS_H

enum class EPaymentStatus
{
  Pending,
  Completed,
  Failed,
  Refunded
};

#endif // E_PAYMENT_STATUS_H