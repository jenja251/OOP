#pragma once
#include "product.h"

class Cake final : public Product {
public:
    Cake(const std::string& name, double pricePerLayer, int layers,
         const std::string& decor, double decorPrice);
    double getPrice() const override;
    void printDetails() const override;
    int getLayers() const;
    const std::string& getDecor() const;
    double getDecorPrice() const;
    void setLayers(int layers);
    void setDecor(const std::string& decor);
    void setDecorPrice(double price);
private:
    int layers_ = 1;
    std::string decor_;
    double decorPrice_ = 0;
};
