#pragma once
#include "order_item.h"
#include <cstddef>
#include <vector>

class Cart {
public:
    void addItem(std::unique_ptr<Product> product, int quantity);
    void removeItem(std::size_t index);
    void setItemQuantity(std::size_t index, int quantity);
    const OrderItem& getItem(std::size_t index) const;
    std::size_t getItemCount() const;
    double getTotalPrice() const;
    void printReceipt() const;
private:
    std::vector<OrderItem> items_;
};
