#pragma once

#include <memory>

namespace patterns::structural {

class NewCalculator {
public:
    virtual double add(double a, double b) const;
    virtual double substract(double a, double b) const;
};

class OldCalculator {
public:
    int sum(int a, int b) const;
};

using UniqueOldCalculator = std::unique_ptr<OldCalculator>;

class Adapter : public NewCalculator {
private:
    UniqueOldCalculator _oldCalculator;

public:
    Adapter(UniqueOldCalculator calc);

    double add(double a, double b) const override;
    double substract(double a, double b) const override;
};

}