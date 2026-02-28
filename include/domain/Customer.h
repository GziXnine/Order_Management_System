#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <stdexcept>

class Customer
{
private:
  int id;
  std::string name;
  double balance;

public:
  Customer();
  Customer(int id, const std::string &name, double balance);

  int getId() const;
  const std::string &getName() const;
  double getBalance() const;

  bool canAfford(double amount) const;
  void deductBalance(double amount);
  void addBalance(double amount);
};

#endif // CUSTOMER_H
