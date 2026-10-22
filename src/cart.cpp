#include "cart.h"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <utility>

void Cart::addItem(std::unique_ptr<Product> product, int quantity) {
    OrderItem item(std::move(product), quantity);
    if (!std::isfinite(getTotalPrice() + item.getTotalPrice()))
        throw std::invalid_argument("Сумма корзины выходит за допустимый диапазон.");
    items_.push_back(std::move(item));
}
void Cart::removeItem(std::size_t index) {
    getItem(index); // Проверка границ до изменения коллекции.
    items_.erase(items_.begin() + static_cast<std::ptrdiff_t>(index));
}
void Cart::setItemQuantity(std::size_t index, int quantity) {
    getItem(index);
    if (quantity <= 0) throw std::invalid_argument("Количество товара должно быть больше нуля.");
    double newTotal = items_[index].getProduct().getPrice() * quantity;
    for (std::size_t i = 0; i < items_.size(); ++i)
        if (i != index) newTotal += items_[i].getTotalPrice();
    if (!std::isfinite(newTotal))
        throw std::invalid_argument("Сумма корзины выходит за допустимый диапазон.");
    items_[index].setQuantity(quantity);
}
const OrderItem& Cart::getItem(std::size_t index) const {
    if (index >= items_.size()) throw std::out_of_range("Строка корзины с таким индексом не существует.");
    return items_[index];
}
std::size_t Cart::getItemCount() const { return items_.size(); }
double Cart::getTotalPrice() const {
    double total = 0;
    for (const auto& item : items_) total += item.getTotalPrice();
    return total;
}
void Cart::printReceipt() const {
    if (items_.empty()) {
        std::cout << "Корзина пуста. Добавьте товары для формирования чека.\n";
        return;
    }
    const auto flags = std::cout.flags();
    const auto precision = std::cout.precision();
    std::cout << std::fixed << std::setprecision(2) << "=== Чек пекарни ===\n";
    for (std::size_t i = 0; i < items_.size(); ++i) {
        const auto& item = items_[i];
        const Product& product = item.getProduct();
        std::cout << i + 1 << ". " << product.getName() << "\n   ";
        product.printDetails(); // Полиморфный вызов через базовый класс.
        std::cout << "\n   " << product.getPrice() << " руб. x " << item.getQuantity()
                  << " = " << item.getTotalPrice() << " руб.\n";
    }
    std::cout << "Итого: " << getTotalPrice() << " руб.\n";
    std::cout.flags(flags);
    std::cout.precision(precision);
}
