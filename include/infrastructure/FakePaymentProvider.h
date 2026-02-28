#ifndef FAKE_PAYMENT_PROVIDER_H
#define FAKE_PAYMENT_PROVIDER_H

#include "../application/Interface/IPaymentProvider.h"

class FakePaymentProvider : public IPaymentProvider
{
private:
  int nextPaymentId;

public:
  FakePaymentProvider();

  bool charge(double amount) override;
  void refund(double amount) override;
  int generatePaymentId() const override;
};

#endif // FAKE_PAYMENT_PROVIDER_H
