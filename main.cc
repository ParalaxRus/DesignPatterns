#include <thread>
#include <vector>

#include "creational/builder/builder.h"
#include "creational/factorymethod/factorymethod.h"
#include "creational/prototype/prototype.h"
#include "creational/singleton/singleton.h"
#include "structural/adapter/adapter.h"
#include "structural/proxy/proxy.h"

namespace {

namespace pc = patterns::creational;
namespace ps = patterns::structural;

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

void builderTest() {
    pc::BbqDinnerBuilder bbqBuilder;
    pc::DinnerDirector director(&bbqBuilder);
    director.liteDinner();

    auto liteDinner = bbqBuilder.getDinner();
    auto liteBbqMeal = liteDinner->menu();
    if (liteBbqMeal != "bbq ribs") {
        throw std::runtime_error("builder failed");
    }

    director.fullDinner();
    auto fullDinner = bbqBuilder.getDinner();
    auto fullBbqMeal = fullDinner->menu();
    if (fullBbqMeal != "shrimp cocktail;bbq ribs;ice cream") {
        throw std::runtime_error("builder failed");
    }
}

void proxyTest() {
    auto clientFunc = [](const ps::Service& service, const std::string& expected) {
        const auto response = service.request();
        if (response != expected) {
            throw std::runtime_error("proxy failed");
        }
    };

    auto serviceA = std::make_unique<ps::ServiceA>("serviceA");
    clientFunc(*serviceA, "serviceA");

    ps::Proxy proxy("proxy", {"proxy"});
    clientFunc(proxy, "proxy");

    ps::Proxy proxy1("proxy1", {"proxy"});
    clientFunc(proxy1, {});
}

void adapterTest() {
    auto add = [](const ps::NewCalculator* calc, double a, double b) -> double {
        return calc->add(a, b);
    };

    auto sub = [](const ps::NewCalculator* calc, double a, double b) -> double {
        return calc->substract(a, b);
    };

    ps::NewCalculator newCalc;
    if (add(&newCalc, 3.5, 4.0) != 7.5) {
        throw std::runtime_error("adapter failed");
    }
    if (sub(&newCalc, 3.5, 4.0) != -0.5) {
        throw std::runtime_error("adapter failed");
    }

    ps::Adapter adapter(std::make_unique<ps::OldCalculator>());
    if (add(&adapter, 3.5, 4.0) != 7.0) {
        throw std::runtime_error("adapter failed");
    }
    if (sub(&adapter, 3.5, 4.0) != -1.0) {
        throw std::runtime_error("adapter failed");
    }
}

}

int main() {
    singletonTest();
    factoryMethodTest();
    prototypeTest();
    builderTest();

    adapterTest();
    proxyTest();

    std::cout << "unit tests pass" << std::endl;

    return 0;
}