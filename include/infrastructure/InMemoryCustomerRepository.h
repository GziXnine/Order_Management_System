#ifndef IN_MEMORY_CUSTOMER_REPOSITORY_H
#define IN_MEMORY_CUSTOMER_REPOSITORY_H

#include <map>
#include "../application/Interface/ICustomerRepository.h"

class InMemoryCustomerRepository : public ICustomerRepository
{
private:
  std::map<int, Customer> customers;

public:
  void save(const Customer &customer) override;
  Customer findById(int customerId) override;
  std::vector<Customer> getAll() const override;

  void addCustomer(const Customer &customer);
};

#endif // IN_MEMORY_CUSTOMER_REPOSITORY_H
