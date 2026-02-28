#include "../../include/domain/Customer.h"

Customer::Customer() : id(0), name(""), balance(0.0) {}

Customer::Customer(int id, const std::string &name, double balance)
    : id(id), name(name), balance(balance)
{
  if (id <= 0)
    throw std::invalid_argument("Customer ID must be positive");
  if (name.empty())
    throw std::invalid_argument("Customer name cannot be empty");
  if (balance < 0)
    throw std::invalid_argument("Customer balance cannot be negative");
}

int Customer::getId() const { return id; }
const std::string &Customer::getName() const { return name; }
double Customer::getBalance() const { return balance; }

bool Customer::canAfford(double amount) const
{
  return balance >= amount;
}

void Customer::deductBalance(double amount)
{
  if (amount <= 0)
    throw std::invalid_argument("Deduction amount must be positive");
  if (amount > balance)
    throw std::runtime_error("Insufficient balance");

  balance -= amount;
}

void Customer::addBalance(double amount)
{
  if (amount <= 0)
    throw std::invalid_argument("Amount must be positive");

  balance += amount;
}
