#include "../../include/domain/OrderItem.h"
#include <stdexcept>

OrderItem::OrderItem() : product(nullptr), quantity(0), lineTotal(0.0) {}
OrderItem::OrderItem(const Product &product, int quantity)
    : product(new Product(product)), quantity(quantity), lineTotal(0.0)
{
  if (quantity <= 0)
    throw std::invalid_argument("Quantity must be at least 1");

  calculateLineTotal();
}

OrderItem::~OrderItem()
{
  delete product;
}

// Copy constructor (deep copy)
OrderItem::OrderItem(const OrderItem &other)
    : product(other.product ? new Product(*other.product) : nullptr),
      quantity(other.quantity),
      lineTotal(other.lineTotal) {}

// Copy assignment (deep copy)
OrderItem &OrderItem::operator=(const OrderItem &other)
{
  if (this != &other)
  {
    delete product;
    product = other.product ? new Product(*other.product) : nullptr;
    quantity = other.quantity;
    lineTotal = other.lineTotal;
  }

  return *this;
}

// Move constructor
OrderItem::OrderItem(OrderItem &&other) noexcept
    : product(other.product), quantity(other.quantity), lineTotal(other.lineTotal)
{
  other.product = nullptr;
  other.quantity = 0;
  other.lineTotal = 0.0;
}

// Move assignment
OrderItem &OrderItem::operator=(OrderItem &&other) noexcept
{
  if (this != &other)
  {
    delete product;
    product = other.product;
    quantity = other.quantity;
    lineTotal = other.lineTotal;
    other.product = nullptr;
    other.quantity = 0;
    other.lineTotal = 0.0;
  }

  return *this;
}

const Product &OrderItem::getProduct() const
{
  if (product == nullptr)
  {
    throw std::runtime_error("Product not set for this OrderItem");
  }

  return *product;
}

int OrderItem::getQuantity() const { return quantity; }
double OrderItem::getLineTotal() const { return lineTotal; }

void OrderItem::setProduct(const Product &product)
{
  if (this->product != nullptr)
  {
    delete this->product; // Clean up existing product if any
  }

  this->product = new Product(product);
  calculateLineTotal();
}

void OrderItem::setQuantity(int quantity)
{
  if (quantity <= 0)
  {
    throw std::invalid_argument("Quantity cannot be zero or negative");
  }

  this->quantity = quantity;
  calculateLineTotal();
}

void OrderItem::calculateLineTotal()
{
  if (product == nullptr)
    lineTotal = 0.0;
  else
    lineTotal = product->getPrice() * quantity;
}

bool OrderItem::operator==(const OrderItem &other) const
{
  if (this->product == nullptr || other.product == nullptr)
    return false;

  // There I Should Compare By Id Because Two Products With Same Id Are Considered The Same Product Even If Other Attributes Differ
  return this->product->getId() == other.product->getId() && this->quantity == other.quantity;
}