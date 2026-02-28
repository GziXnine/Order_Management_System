#include "../../include/domain/Product.h"

Product::Product() : id(0), name(""), price(0.0), active(true) {}
Product::Product(int id, const std::string &name, double price, bool active)
    : id(id), name(name), price(price), active(active) {}

int Product::getId() const { return id; }
const std::string &Product::getName() const { return name; }
double Product::getPrice() const { return price; }
bool Product::isActive() const { return active; }

void Product::setName(const std::string &name)
{
  this->name = name;
}

void Product::setPrice(double price)
{
  this->price = price;
}

void Product::setActive(bool active)
{
  this->active = active;
}

void Product::updatePrice(double newPrice)
{
  if (newPrice < 0)
  {
    throw std::invalid_argument("Price cannot be negative");
  }

  price = newPrice;
}