#include "bread.h"
#include "cake.h"
#include "cart.h"
#include "pastry.h"
#include "product_factory.h"
#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>

namespace {
int checks = 0;
void check(bool condition, const char* message) {
    ++checks;
    if (!condition) throw std::runtime_error(message);
}
template<class Exception = std::invalid_argument, class Function>
void rejects(Function action) {
    bool rejected = false;
    try { action(); } catch (const Exception&) { rejected = true; }
    check(rejected, "Expected exception was not thrown");
}
auto bread() { return ProductFactory::createProduct("bread", "Bread", 120, "rye", 0.5); }
}
int main() {
    try {
        check(bread()->getPrice() == 60, "Bread price");
        check(ProductFactory::createProduct("pastry", "Pastry", 65, "cherry", 15)->getPrice() == 80, "Pastry price");
        check(ProductFactory::createProduct("cake", "Cake", 300, "berries", 200, 3)->getPrice() == 1100, "Cake price");
        rejects([] { ProductFactory::createProduct("unknown", "Name", 1, "detail", 1); });
        rejects([] { Bread("Bread", -1, "rye", 1); });
        rejects([] { Bread(" ", 1, "rye", 1); });
        rejects([] { Bread("Bread", 1, "", 1); });
        rejects([] { Bread("Bread", 1, "rye", 0); });
        rejects([] { Bread("Bread", 1, "rye", -1); });
        rejects([] { Bread("Bread", 1, "rye", std::numeric_limits<double>::quiet_NaN()); });
        rejects([] { Pastry("Pastry", 1, "", 1); });
        rejects([] { Pastry("Pastry", 1, "cherry", -1); });
        rejects([] { Cake("Cake", 1, 0, "berries", 1); });
        rejects([] { Cake("Cake", 1, 1, "", 1); });
        rejects([] { Cake("Cake", 1, 1, "berries", -1); });
        rejects([] { Cake("Cake", std::numeric_limits<double>::infinity(), 1, "berries", 1); });
        Bread mutableBread("Bread", 120, "rye", 0.5);
        rejects([&] { mutableBread.setWeightKg(0); });
        check(mutableBread.getWeightKg() == 0.5, "Failed setter changed state");
        mutableBread.setWeightKg(1);
        check(mutableBread.getPrice() == 120, "Setter did not affect price");
        Cart cart;
        check(cart.getTotalPrice() == 0, "Empty total");
        rejects([&] { cart.addItem(nullptr, 1); });
        rejects([&] { cart.addItem(bread(), 0); });
        rejects([&] { cart.addItem(bread(), -2); });
        check(cart.getItemCount() == 0, "Failed insertion changed cart");
        cart.addItem(bread(), 2);
        cart.addItem(ProductFactory::createProduct("pastry", "Pastry", 65, "cherry", 15), 3);
        check(cart.getTotalPrice() == 360, "Cart total");
        cart.setItemQuantity(0, 3);
        check(cart.getItem(0).getTotalPrice() == 180 && cart.getTotalPrice() == 420, "Quantity recalculation");
        rejects([&] { cart.setItemQuantity(0, 0); });
        check(cart.getTotalPrice() == 420, "Invalid quantity changed total");
        rejects<std::out_of_range>([&] { cart.removeItem(2); });
        rejects<std::out_of_range>([&] { cart.setItemQuantity(2, 1); });
        cart.removeItem(0);
        check(cart.getTotalPrice() == 240 && cart.getItemCount() == 1, "Removal");
        cart.addItem(ProductFactory::createProduct("cake", "Cake", 300, "berries", 200, 3), 1);
        cart.addItem(bread(), 1);
        std::ostringstream receipt;
        auto* oldBuffer = std::cout.rdbuf(receipt.rdbuf());
        cart.printReceipt();
        Cart empty;
        empty.printReceipt();
        std::cout.rdbuf(oldBuffer);
        check(receipt.str().find("cherry") != std::string::npos, "Pastry details missing");
        check(receipt.str().find("berries") != std::string::npos, "Cake details missing");
        check(receipt.str().find("rye") != std::string::npos, "Bread details missing");
        check(receipt.str().find("1400.00") != std::string::npos, "Receipt total missing");
        check(receipt.str().find("Корзина пуста") != std::string::npos, "Empty receipt missing");
        Cart overflow;
        overflow.addItem(ProductFactory::createProduct("pastry", "Big", std::numeric_limits<double>::max(), "plain", 0), 1);
        rejects([&] { overflow.setItemQuantity(0, 2); });
        rejects([&] { overflow.addItem(ProductFactory::createProduct("pastry", "Big", std::numeric_limits<double>::max(), "plain", 0), 1); });
        check(overflow.getItemCount() == 1 && std::isfinite(overflow.getTotalPrice()), "Overflow damaged cart");
        std::cout << "PASS: " << checks << " checks\n";
    } catch (const std::exception& error) {
        std::cerr << "FAIL: " << error.what() << "\n";
        return 1;
    }
}
