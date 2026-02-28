#include "../../include/infrastructure/FakePaymentProvider.h"
#include <iostream>

FakePaymentProvider::FakePaymentProvider() : nextPaymentId(1000) {}

bool FakePaymentProvider::charge(double amount)
{
  if (amount <= 0.0)
  {
    std::cout << "[Payment] Charge rejected: invalid amount " << amount << "\n";
    return false;
  }

  std::cout << "[Payment] Charged $" << amount << " successfully.\n";
  return true;
}

void FakePaymentProvider::refund(double amount)
{
  std::cout << "[Payment] Refunded $" << amount << ".\n";
}

int FakePaymentProvider::generatePaymentId() const
{
  return nextPaymentId;
}
