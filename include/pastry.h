#pragma once
#include "product.h"

class Pastry final : public Product {
public:
    Pastry(const std::string& name, double basePrice,
           const std::string& filling, double fillingSurcharge);
    double getPrice() const override;
    void printDetails() const override;
    std::unique_ptr<Product> clone() const override;
    const std::string& getFilling() const;
    double getFillingSurcharge() const;
    void setFilling(const std::string& filling);
    void setFillingSurcharge(double surcharge);
private:
    std::string filling_;
    double fillingSurcharge_ = 0;
};
