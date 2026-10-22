#include "cake.h"
#include <cmath>
#include <iostream>
#include <stdexcept>

Cake::Cake(const std::string& name, double pricePerLayer, int layers,
           const std::string& decor, double decorPrice)
    : Product(name, pricePerLayer) {
    setLayers(layers);
    setDecor(decor);
    setDecorPrice(decorPrice);
}
double Cake::getPrice() const { return getBasePrice() * layers_ + decorPrice_; }
std::unique_ptr<Product> Cake::clone() const { return std::make_unique<Cake>(*this); }
void Cake::printDetails() const {
    std::cout << "Слоёв: " << layers_ << ", декор: " << decor_;
}
int Cake::getLayers() const { return layers_; }
const std::string& Cake::getDecor() const { return decor_; }
double Cake::getDecorPrice() const { return decorPrice_; }
void Cake::setLayers(int layers) {
    if (layers <= 0) throw std::invalid_argument("Количество слоёв должно быть больше нуля.");
    layers_ = layers;
}
void Cake::setDecor(const std::string& decor) {
    if (decor.find_first_not_of(" \t\n\r") == std::string::npos)
        throw std::invalid_argument("Декор не может быть пустым; укажите «без декора».");
    decor_ = decor;
}
void Cake::setDecorPrice(double price) {
    if (!std::isfinite(price) || price < 0)
        throw std::invalid_argument("Цена декора должна быть конечным неотрицательным числом.");
    decorPrice_ = price;
}
