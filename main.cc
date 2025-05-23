#include <vector>
#include <thread>

#include "singleton/singleton.h"

namespace {

namespace pt = patterns::singleton;

void createSingleton(std::vector<pt::Singleton*>& instances, std::mutex& mtx) {
    std::lock_guard<std::mutex> lock(mtx);
    instances.push_back(pt::Singleton::get());
}

void singletonTest() {
    std::mutex mtx;
    std::vector<pt::Singleton*> instances = {};
    std::vector<std::thread> threads;

    constexpr int num = 10;
    for (int i = 0; i < num; ++i) {
        threads.emplace_back(createSingleton, std::ref(instances), std::ref(mtx));
    }

    for (auto& t : threads) {
        t.join();
    }

    if (instances.size() != num) {
        throw std::runtime_error("Singleton pattern failed");
    }

    for (size_t i = 1; i < instances.size(); ++i) {
        if (instances[i] != instances[i-1]) {
            throw std::runtime_error("Singleton pattern failed");    
        }
    }
}

}

int main() {
    singletonTest();

    std::cout << "unit tests pass" << std::endl;

    return 0;
}