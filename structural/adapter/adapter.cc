#include "adapter.h"

namespace patterns::structural {

double NewCalculator::add(double a, double b) const {
    return a + b;
}

double NewCalculator::substract(double a, double b) const {
    return a - b;
}

int OldCalculator::sum(int a, int b) const {
    return a + b;
}

Adapter::Adapter(UniqueOldCalculator calc) : _oldCalculator(std::move(calc)) {
}

double Adapter::add(double a, double b) const {
    return this->_oldCalculator->sum(a, b);
}

double Adapter::substract(double a, double b) const {
    return this->_oldCalculator->sum(a, -b);
}


}