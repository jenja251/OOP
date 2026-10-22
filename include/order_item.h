#pragma once
#include "product.h"
#include <memory>

class OrderItem {
public:
    OrderItem(std::unique_ptr<Product> product, int quantity);
    const Product& getProduct() const;
    int getQuantity() const;
    double getTotalPrice() const;
    void setQuantity(int quantity);
private:
    std::unique_ptr<Product> product_;
    int quantity_ = 1;
    double totalPrice_ = 0;
};
