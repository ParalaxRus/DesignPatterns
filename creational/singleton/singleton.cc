#include "singleton.h"

namespace patterns::creational {

std::unique_ptr<Singleton> patterns::creational::Singleton::_singleton = nullptr;
std::mutex Singleton::_mtx;

Singleton* Singleton::get() {
    if (!Singleton::_singleton) {
        std::lock_guard<std::mutex> lock(Singleton::_mtx);
        if (!Singleton::_singleton) {
            Singleton::_singleton = std::unique_ptr<Singleton>(new Singleton());
        }
    }
    return Singleton::_singleton.get();
}

}
