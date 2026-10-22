#include "pastry.h"
#include <cmath>
#include <iostream>
#include <stdexcept>

Pastry::Pastry(const std::string& name, double basePrice,
               const std::string& filling, double fillingSurcharge)
    : Product(name, basePrice) {
    setFilling(filling);
    setFillingSurcharge(fillingSurcharge);
}
double Pastry::getPrice() const { return getBasePrice() + fillingSurcharge_; }
std::unique_ptr<Product> Pastry::clone() const { return std::make_unique<Pastry>(*this); }
void Pastry::printDetails() const { std::cout << "Начинка: " << filling_; }
const std::string& Pastry::getFilling() const { return filling_; }
double Pastry::getFillingSurcharge() const { return fillingSurcharge_; }
void Pastry::setFilling(const std::string& filling) {
    if (filling.find_first_not_of(" \t\n\r") == std::string::npos)
        throw std::invalid_argument("Начинка не может быть пустой.");
    filling_ = filling;
}
void Pastry::setFillingSurcharge(double surcharge) {
    if (!std::isfinite(surcharge) || surcharge < 0)
        throw std::invalid_argument("Доплата за начинку должна быть конечным неотрицательным числом.");
    fillingSurcharge_ = surcharge;
}
