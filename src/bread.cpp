#include "bread.h"
#include <cmath>
#include <iostream>
#include <stdexcept>

Bread::Bread(const std::string& name, double pricePerKg,
             const std::string& doughType, double weightKg)
    : Product(name, pricePerKg) {
    setDoughType(doughType);
    setWeightKg(weightKg);
}
double Bread::getPrice() const { return getBasePrice() * weightKg_; }
void Bread::printDetails() const {
    std::cout << "Тесто: " << doughType_ << ", вес: " << weightKg_ << " кг";
}
const std::string& Bread::getDoughType() const { return doughType_; }
double Bread::getWeightKg() const { return weightKg_; }
void Bread::setDoughType(const std::string& type) {
    if (type.find_first_not_of(" \t\n\r") == std::string::npos)
        throw std::invalid_argument("Тип теста не может быть пустым.");
    doughType_ = type;
}
void Bread::setWeightKg(double weight) {
    if (!std::isfinite(weight) || weight <= 0)
        throw std::invalid_argument("Вес хлеба должен быть конечным числом больше нуля.");
    weightKg_ = weight;
}
