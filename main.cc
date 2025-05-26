#include <thread>
#include <vector>

#include "creational/factorymethod/factorymethod.h"
#include "creational/prototype/prototype.h"
#include "creational/singleton/singleton.h"

namespace {

namespace pc = patterns::creational;

void createSingleton(std::vector<pc::Singleton*>& instances, std::mutex& mtx) {
    std::lock_guard<std::mutex> lock(mtx);
    instances.push_back(pc::Singleton::get());
}

void singletonTest() {
    std::mutex mtx;
    std::vector<pc::Singleton*> instances = {};
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

void factoryMethodTest() {
    std::vector<std::unique_ptr<pc::Factory>> factories;
    factories.push_back(std::make_unique<pc::TriangleFactory>());
    factories.push_back(std::make_unique<pc::RectangleFactory>());
    
    std::string res = "";
    for (const auto& f : factories) {
        res += f->doWork() + "; ";
    }

    if (res != "name: triangle; name: rectangle; ") {
        throw std::runtime_error("factory method failed");
    }
}

void prototypeTest() {
    pc::PrototypeFactory factory;
    auto protoB = factory.create(pc::PrototypeType::PrototypeTypeB);

    std::vector<pc::UniquePrototype> prototypes;
    prototypes.emplace_back(factory.create(pc::PrototypeType::PrototypeTypeB));
    prototypes.emplace_back(factory.create(pc::PrototypeType::PrototypeTypeA));
    
    std::string res;
    for (const auto& p : prototypes) {
        res += p->representation() + "/n";
    }
    
    if (res != "Name: PrototypeB Val: 2 ValB: 2/nName: PrototypeA Val: 1 ValA: 1/n") {
        throw std::runtime_error("prototype failed");
    }
}

}

int main() {
    singletonTest();
    factoryMethodTest();
    prototypeTest();

    std::cout << "unit tests pass" << std::endl;

    return 0;
}