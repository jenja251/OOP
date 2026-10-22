#pragma once
#include <string>
#include <memory>

class Product {
public:
    Product(const std::string& name, double basePrice);
    virtual ~Product() = default;
    virtual double getPrice() const = 0;
    virtual std::string getName() const;
    virtual void printDetails() const = 0;
    virtual std::unique_ptr<Product> clone() const = 0;
    double getBasePrice() const;
    void setName(const std::string& name);
    void setBasePrice(double price);
private:
    std::string name_;
    double basePrice_ = 0;
};
