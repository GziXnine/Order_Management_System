#include "../../include/infrastructure/InMemoryProductRepository.h"
#include <stdexcept>

void InMemoryProductRepository::addProduct(const Product &product)
{
  products[product.getId()] = product;
}

Product InMemoryProductRepository::findById(int productId)
{
  auto it = products.find(productId);
  if (it == products.end())
    throw std::runtime_error("Product not found with id: " + std::to_string(productId));

  return it->second;
}

std::vector<Product> InMemoryProductRepository::getAll() const
{
  std::vector<Product> result;
  for (const auto &pair : products)
    result.push_back(pair.second);

  return result;
}
