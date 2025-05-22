#include "singleton/singleton.h"

int main() {

    const auto* single1 = patterns::singleton::Singleton::get();
    if (single1 == nullptr) {
        throw std::runtime_error("Singleton pattern failed");
    }
    single1->hi();

    const auto* single2 = patterns::singleton::Singleton::get();
    if (single1 != single2) {
        throw std::runtime_error("Singleton pattern failed");
    }
    single2->hi();

    return 0;
}