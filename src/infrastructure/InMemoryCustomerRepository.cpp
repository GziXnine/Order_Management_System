#include "../../include/infrastructure/InMemoryCustomerRepository.h"
#include <stdexcept>

void InMemoryCustomerRepository::save(const Customer &customer)
{
  customers[customer.getId()] = customer;
}

Customer InMemoryCustomerRepository::findById(int customerId)
{
  auto it = customers.find(customerId);
  if (it == customers.end())
    throw std::runtime_error("Customer not found with ID: " + std::to_string(customerId));

  return it->second;
}

std::vector<Customer> InMemoryCustomerRepository::getAll() const
{
  std::vector<Customer> result;
  for (const auto &pair : customers)
    result.push_back(pair.second);

  return result;
}

void InMemoryCustomerRepository::addCustomer(const Customer &customer)
{
  customers[customer.getId()] = customer;
}
