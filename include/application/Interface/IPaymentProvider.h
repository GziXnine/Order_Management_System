#ifndef IPAYMENT_PROVIDER_H
#define IPAYMENT_PROVIDER_H

class IPaymentProvider
{
public:
  virtual bool charge(double amount) = 0;
  virtual void refund(double amount) = 0;
  // Optional helper: allow provider to generate an identifier for local records.
  virtual int generatePaymentId() const { return 0; }

  virtual ~IPaymentProvider() = default;
};

#endif