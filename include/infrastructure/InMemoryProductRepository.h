#ifndef IN_MEMORY_PRODUCT_REPOSITORY_H
#define IN_MEMORY_PRODUCT_REPOSITORY_H

#include <map>
#include "../application/Interface/IProductRepository.h"

class InMemoryProductRepository : public IProductRepository
{
private:
  std::map<int, Product> products;

public:
  void addProduct(const Product &product);
  Product findById(int productId) override;
  std::vector<Product> getAll() const override;
};

#endif // IN_MEMORY_PRODUCT_REPOSITORY_H
