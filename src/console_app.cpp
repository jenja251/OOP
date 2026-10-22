#include "console_app.h"
#include "cart.h"
#include "product_factory.h"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

namespace {
class EndOfInput final : public std::exception {};

std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    if (!std::getline(std::cin, line)) throw EndOfInput{};
    return line;
}

template<class Number>
Number readNumber(const std::string& prompt) {
    auto line = readLine(prompt);
    if constexpr (std::is_floating_point_v<Number>)
        std::replace(line.begin(), line.end(), ',', '.');
    std::istringstream input(line);
    Number value{};
    if (!(input >> value)) throw std::invalid_argument("Введите корректное число.");
    input >> std::ws;
    if (!input.eof()) throw std::invalid_argument("После числа не должно быть других символов.");
    if constexpr (std::is_floating_point_v<Number>)
        if (!std::isfinite(value)) throw std::invalid_argument("Число должно быть конечным.");
    return value;
}

std::size_t readIndex(const std::string& prompt, std::size_t count) {
    const int number = readNumber<int>(prompt);
    if (number <= 0 || static_cast<std::size_t>(number) > count)
        throw std::out_of_range("Товара с таким номером нет.");
    return static_cast<std::size_t>(number - 1);
}

using Catalog = std::vector<std::unique_ptr<Product>>;

void printCatalog(const Catalog& catalog) {
    if (catalog.empty()) {
        std::cout << "Каталог пуст. Создайте товар через пункт 2.\n";
        return;
    }
    std::cout << "=== Каталог товаров ===\n";
    for (std::size_t i = 0; i < catalog.size(); ++i) {
        std::cout << i + 1 << ". " << catalog[i]->getName() << " — "
                  << std::fixed << std::setprecision(2) << catalog[i]->getPrice() << " руб.\n   ";
        catalog[i]->printDetails();
        std::cout << '\n';
    }
}

std::unique_ptr<Product> readProduct() {
    const int type = readNumber<int>("Тип товара (1 — хлеб, 2 — выпечка, 3 — торт): ");
    if (type < 1 || type > 3) throw std::invalid_argument("Выберите тип от 1 до 3.");
    const auto name = readLine("Название: ");
    if (type == 1) {
        const auto price = readNumber<double>("Цена за кг, руб.: ");
        const auto dough = readLine("Тип теста: ");
        const auto weight = readNumber<double>("Вес, кг: ");
        return ProductFactory::createProduct("bread", name, price, dough, weight);
    }
    if (type == 2) {
        const auto price = readNumber<double>("Базовая цена штуки, руб.: ");
        const auto filling = readLine("Начинка: ");
        const auto surcharge = readNumber<double>("Доплата за начинку, руб.: ");
        return ProductFactory::createProduct("pastry", name, price, filling, surcharge);
    }
    const auto price = readNumber<double>("Цена за слой, руб.: ");
    const auto layers = readNumber<int>("Количество слоёв: ");
    const auto decor = readLine("Декор (или «без декора»): ");
    const auto decorPrice = readNumber<double>("Цена декора, руб.: ");
    return ProductFactory::createProduct("cake", name, price, decor, decorPrice, layers);
}
}

void runInteractive() {
    Catalog catalog;
    catalog.push_back(ProductFactory::createProduct("bread", "Хлеб ржаной", 120, "ржаное", 0.5));
    catalog.push_back(ProductFactory::createProduct("pastry", "Слойка", 65, "вишня", 15));
    catalog.push_back(ProductFactory::createProduct("cake", "Торт праздничный", 300, "ягоды", 200, 3));
    Cart cart;
    std::cout << "Система заказов пекарни\nВ каталоге есть три примера товаров. Корзина пуста.\n";
    for (;;) {
        std::cout << "\n1 — Каталог товаров\n2 — Создать товар\n3 — Удалить товар из каталога\n"
                     "4 — Добавить товар в корзину\n5 — Корзина и чек\n"
                     "6 — Изменить количество в корзине\n7 — Удалить строку из корзины\n0 — Выход\n";
        try {
            switch (readNumber<int>("Выберите действие: ")) {
            case 0:
                std::cout << "До свидания!\n";
                return;
            case 1:
                printCatalog(catalog);
                break;
            case 2: {
                auto product = readProduct();
                if (!std::isfinite(product->getPrice()))
                    throw std::invalid_argument("Цена товара выходит за допустимый диапазон.");
                catalog.push_back(std::move(product));
                std::cout << "Товар создан. Для покупки добавьте его в корзину через пункт 4.\n";
                break;
            }
            case 3: {
                printCatalog(catalog);
                if (catalog.empty()) break;
                const auto index = readIndex("Номер товара для удаления: ", catalog.size());
                catalog.erase(catalog.begin() + static_cast<std::ptrdiff_t>(index));
                std::cout << "Товар удалён из каталога. Ранее добавленные строки корзины сохранены.\n";
                break;
            }
            case 4: {
                printCatalog(catalog);
                if (catalog.empty()) break;
                const auto index = readIndex("Номер товара: ", catalog.size());
                const auto quantity = readNumber<int>("Количество: ");
                cart.addItem(catalog[index]->clone(), quantity);
                std::cout << "Товар добавлен в корзину.\n";
                break;
            }
            case 5:
                cart.printReceipt();
                break;
            case 6: {
                cart.printReceipt();
                if (cart.getItemCount() == 0) break;
                const auto index = readIndex("Номер строки корзины: ", cart.getItemCount());
                const auto quantity = readNumber<int>("Новое количество: ");
                cart.setItemQuantity(index, quantity);
                std::cout << "Количество изменено.\n";
                break;
            }
            case 7: {
                cart.printReceipt();
                if (cart.getItemCount() == 0) break;
                const auto index = readIndex("Номер строки для удаления: ", cart.getItemCount());
                cart.removeItem(index);
                std::cout << "Строка удалена из корзины.\n";
                break;
            }
            default:
                std::cout << "Выберите действие от 0 до 7.\n";
            }
        } catch (const EndOfInput&) {
            std::cout << "\nВвод завершён. До свидания!\n";
            return;
        } catch (const std::invalid_argument& error) {
            std::cout << "Ошибка: " << error.what() << " Возврат в меню.\n";
        } catch (const std::out_of_range& error) {
            std::cout << "Ошибка: " << error.what() << " Возврат в меню.\n";
        }
    }
}
