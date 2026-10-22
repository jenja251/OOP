#pragma once
#include "product.h"
#include <memory>
#include <string>

class ProductFactory {
public:
    // detail: тесто / начинка / декор; value: вес в кг / доплата за начинку / цена декора.
    // basePrice: цена за кг / базовая цена штуки / цена за слой. layers нужен только cake.
    static std::unique_ptr<Product> createProduct(
        const std::string& type, const std::string& name, double basePrice,
        const std::string& detail, double value, int layers = 1);
};
