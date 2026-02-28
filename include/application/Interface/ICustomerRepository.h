#ifndef ICUSTOMER_REPOSITORY_H
#define ICUSTOMER_REPOSITORY_H

#include "../../domain/Customer.h"
#include <vector>

class ICustomerRepository
{
public:
  virtual void save(const Customer &customer) = 0;
  virtual Customer findById(int customerId) = 0;
  virtual std::vector<Customer> getAll() const = 0;

  virtual ~ICustomerRepository() = default;
};

#endif // ICUSTOMER_REPOSITORY_H
