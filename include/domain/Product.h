#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <stdexcept>

class Product
{
private:
  int id;
  std::string name;
  double price;
  bool active;

public:
  Product();
  Product(int id, const std::string &name, double price, bool active);

  int getId() const;
  const std::string &getName() const;
  double getPrice() const;
  bool isActive() const;

  void setName(const std::string &name);
  void setPrice(double price);
  void setActive(bool active);

  void updatePrice(double newPrice);
};

#endif // PRODUCT_H