#pragma once
#include "product.h"

class Bread final : public Product {
public:
    Bread(const std::string& name, double pricePerKg,
          const std::string& doughType, double weightKg);
    double getPrice() const override;
    void printDetails() const override;
    const std::string& getDoughType() const;
    double getWeightKg() const;
    void setDoughType(const std::string& type);
    void setWeightKg(double weight);
private:
    std::string doughType_;
    double weightKg_ = 1;
};
