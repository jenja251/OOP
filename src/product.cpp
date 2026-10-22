#include "product.h"
#include <cmath>
#include <stdexcept>

Product::Product(const std::string& name, double basePrice) {
    setName(name);
    setBasePrice(basePrice);
}
std::string Product::getName() const { return name_; }
double Product::getBasePrice() const { return basePrice_; }
void Product::setName(const std::string& name) {
    if (name.find_first_not_of(" \t\n\r") == std::string::npos)
        throw std::invalid_argument("Название товара не может быть пустым.");
    name_ = name;
}
void Product::setBasePrice(double price) {
    if (!std::isfinite(price) || price < 0)
        throw std::invalid_argument("Базовая цена должна быть конечным неотрицательным числом.");
    basePrice_ = price;
}
