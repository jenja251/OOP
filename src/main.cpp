#include "cart.h"
#include "product_factory.h"
#include "console_app.h"
#include <exception>
#include <iostream>
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

int runDemo() {
    try {
        Cart cart;
        cart.addItem(ProductFactory::createProduct("bread", "Хлеб ржаной", 120, "ржаное", 0.5), 2);
        cart.addItem(ProductFactory::createProduct("pastry", "Слойка", 65, "вишня", 15), 3);
        cart.addItem(ProductFactory::createProduct("cake", "Торт праздничный", 300, "ягоды", 200, 3), 1);
        cart.printReceipt();

        std::cout << "\nПроверка некорректного количества:\n";
        try {
            cart.addItem(ProductFactory::createProduct("bread", "Батон", 100, "пшеничное", 0.4), 0);
        } catch (const std::invalid_argument& error) {
            std::cout << "Ошибка: " << error.what() << "\n";
        }
        std::cout << "\nПроверка некорректного товара:\n";
        try {
            cart.addItem(ProductFactory::createProduct("bread", "Батон", 100, "пшеничное", 0), 1);
        } catch (const std::invalid_argument& error) {
            std::cout << "Ошибка: " << error.what() << "\n";
        }
        std::cout << "\nПроверка неизвестного типа:\n";
        try {
            cart.addItem(ProductFactory::createProduct("pizza", "Пицца", 100, "сыр", 20), 1);
        } catch (const std::invalid_argument& error) {
            std::cout << "Ошибка: " << error.what() << "\n";
        }

        std::cout << "\nПрограмма продолжает работу. Меняем количество слоек с 3 на 2:\n";
        cart.setItemQuantity(1, 2);
        cart.printReceipt();
        std::cout << "\nУдаляем торт (индекс 2). Осталось строк: ";
        cart.removeItem(2);
        std::cout << cart.getItemCount() << "\n";

        std::cout << "\nПроверка пустой корзины:\n";
        Cart emptyCart;
        emptyCart.printReceipt();
    } catch (const std::exception& error) {
        std::cerr << "Непредвиденная ошибка: " << error.what() << "\n";
        return 1;
    }
    return 0;
}

int main(int argc, char* argv[]) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    try {
        if (argc == 2 && std::string(argv[1]) == "--demo") return runDemo();
        if (argc != 1) {
            std::cout << "Запуск: bakery [--demo]\nБез аргументов — интерактивное меню.\n";
            return 1;
        }
        runInteractive();
    } catch (const std::exception& error) {
        std::cerr << "Непредвиденная ошибка: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
