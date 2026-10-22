#include "product_factory.h"
#include "bread.h"
#include "pastry.h"
#include "cake.h"
#include <stdexcept>

std::unique_ptr<Product> ProductFactory::createProduct(
    const std::string& type, const std::string& name, double basePrice,
    const std::string& detail, double value, int layers) {
    if (type == "bread") return std::make_unique<Bread>(name, basePrice, detail, value);
    if (type == "pastry") return std::make_unique<Pastry>(name, basePrice, detail, value);
    if (type == "cake") return std::make_unique<Cake>(name, basePrice, layers, detail, value);
    throw std::invalid_argument("Неизвестный тип товара: " + type);
}
