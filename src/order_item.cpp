#include "order_item.h"
#include <cmath>
#include <stdexcept>
#include <utility>

OrderItem::OrderItem(std::unique_ptr<Product> product, int quantity)
    : product_(std::move(product)) {
    if (!product_) throw std::invalid_argument("Товар не может быть nullptr.");
    setQuantity(quantity);
}
const Product& OrderItem::getProduct() const { return *product_; }
int OrderItem::getQuantity() const { return quantity_; }
double OrderItem::getTotalPrice() const { return totalPrice_; }
void OrderItem::setQuantity(int quantity) {
    if (quantity <= 0) throw std::invalid_argument("Количество товара должно быть больше нуля.");
    const double total = product_->getPrice() * quantity;
    if (!std::isfinite(total) || total < 0)
        throw std::invalid_argument("Стоимость строки выходит за допустимый диапазон.");
    quantity_ = quantity;
    totalPrice_ = total;
}
