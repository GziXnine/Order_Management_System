#ifndef IPRODUCT_REPOSITORY_H
#define IPRODUCT_REPOSITORY_H

#include "../../domain/Product.h"
#include <vector>

class IProductRepository
{
public:
  virtual Product findById(int productId) = 0;
  virtual std::vector<Product> getAll() const = 0;

  virtual ~IProductRepository() = default;
};

#endif // IPRODUCT_REPOSITORY_H
