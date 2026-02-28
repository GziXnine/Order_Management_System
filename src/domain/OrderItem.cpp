#include "../../include/domain/OrderItem.h"

OrderItem::OrderItem() : product(nullptr), quantity(0), lineTotal(0.0) {}
OrderItem::OrderItem(const Product &product, int quantity)
    : product(new Product(product)), quantity(quantity), lineTotal(0.0)
{
  calculateLineTotal();
}

OrderItem::~OrderItem()
{
  delete product;
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