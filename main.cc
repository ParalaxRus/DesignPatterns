#include <thread>
#include <vector>

#include "creational/builder/builder.h"
#include "creational/factorymethod/factorymethod.h"
#include "creational/prototype/prototype.h"
#include "creational/singleton/singleton.h"
#include "pointers/shared/custom_shared_ptr.h"
#include "pointers/unique/custom_unique_ptr.h"
#include "structural/adapter/adapter.h"
#include "structural/proxy/proxy.h"

namespace {

namespace pc = patterns::creational;
namespace pp = patterns::pointers;
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

void uniquePtrTest() {
    // Ctor
    pp::CustomUniquePtr<int> uniquePtr(new int(5));
    if ((uniquePtr.get() == nullptr) || (*uniquePtr.get() != 5)) {
        throw std::runtime_error("unique pointer failed");    
    }

    // Move ctor
    pp::CustomUniquePtr<int> uniquePtr1(std::move(uniquePtr));
    if (uniquePtr) {
        throw std::runtime_error("unique pointer failed");    
    }
    if ( !uniquePtr1 || (*uniquePtr1.get() != 5)) {
        throw std::runtime_error("unique pointer failed");    
    }

    // Move copy assignment
    auto uniquePtr2 = std::move(uniquePtr1);
    if (uniquePtr1) {
        throw std::runtime_error("unique pointer failed");    
    }
    if ( !uniquePtr2 || (*uniquePtr2.get() != 5)) {
        throw std::runtime_error("unique pointer failed");    
    }
    if (*uniquePtr2 != 5) {
        throw std::runtime_error("unique pointer failed");
    }

    // Container test
    std::vector<pp::CustomUniquePtr<double>> uniquePtrs;
    uniquePtrs.push_back(pp::CustomUniquePtr<double>(new double(1.1)));
    uniquePtrs.push_back(pp::CustomUniquePtr<double>(new double(2.2)));
    uniquePtrs.push_back(pp::CustomUniquePtr<double>(new double(3.3)));

    const std::vector<double> expected = {1.1, 2.2, 3.3};
    int i = 0;
    for (const auto& ptr : uniquePtrs) {
        if (*ptr != expected[i++]) {
            throw std::runtime_error("unique pointer failed");
        }
    }
}

void sharedPtrTest() {
    // Ctor
    pp::CustomSharedPtr<int> sharedPtr(new int(5));
    if (!sharedPtr || (*sharedPtr != 5) ) {
        throw std::runtime_error("shared pointer failed");
    }

    // Copy ctor
    pp::CustomSharedPtr<int> sharedPtr1(sharedPtr);
    if ( !sharedPtr || (*sharedPtr != 5) || !sharedPtr1 || (*sharedPtr1 != 5) ) {
        throw std::runtime_error("shared pointer failed");
    }

    sharedPtr.release();
    if (sharedPtr) {
        throw std::runtime_error("shared pointer failed");
    }
    if (!sharedPtr1 || (*sharedPtr1 != 5) ) {
        throw std::runtime_error("shared pointer failed");
    }
    sharedPtr1.release();
    if (sharedPtr1) {
        throw std::runtime_error("shared pointer failed");
    }

    // Copy assignment
    pp::CustomSharedPtr<int> sharedPtr2(new int(10));
    auto sharedPtr3 = sharedPtr2;
    if ( !sharedPtr2 || (*sharedPtr2 != 10) || !sharedPtr3 || (*sharedPtr3 != 10) ) {
        throw std::runtime_error("shared pointer failed");
    }

    sharedPtr2.release();
    if (sharedPtr2) {
        throw std::runtime_error("shared pointer failed");
    }
    if (!sharedPtr3 || (*sharedPtr3 != 10) ) {
        throw std::runtime_error("shared pointer failed");
    }
    sharedPtr3.release();
    if (sharedPtr3) {
        throw std::runtime_error("shared pointer failed");
    }

    // Move ctor
    pp::CustomSharedPtr<int> sharedPtr4(new int(2));
    pp::CustomSharedPtr<int> sharedPtr5(std::move(sharedPtr4));
    if (sharedPtr4) {
        throw std::runtime_error("shared pointer failed");
    }
    if (!sharedPtr5 || (*sharedPtr5 != 2) ) {
        throw std::runtime_error("shared pointer failed");
    }
    sharedPtr5.release();
    if (sharedPtr5) {
        throw std::runtime_error("shared pointer failed");
    }

    // Move assignment
    pp::CustomSharedPtr<int> sharedPtr6(new int(3));
    auto sharedPtr7 = std::move(sharedPtr6);
    if (sharedPtr6) {
        throw std::runtime_error("shared pointer failed");
    }
    if (!sharedPtr7 || (*sharedPtr7 != 3) ) {
        throw std::runtime_error("shared pointer failed");
    }
    sharedPtr7.release();
    if (sharedPtr7) {
        throw std::runtime_error("shared pointer failed");
    }

    // Container test 1
    std::vector<pp::CustomSharedPtr<double>> sharedPtrs;
    sharedPtrs.push_back(pp::CustomSharedPtr<double>(new double(1.1)));
    sharedPtrs.push_back(pp::CustomSharedPtr<double>(new double(2.2)));
    sharedPtrs.push_back(pp::CustomSharedPtr<double>(new double(3.3)));

    const std::vector<double> expected = {1.1, 2.2, 3.3};
    int i = 0;
    for (const auto& ptr : sharedPtrs) {
        if (*ptr != expected[i++]) {
            throw std::runtime_error("shared pointer failed");
        }
    }

    // Container test 2
    pp::CustomSharedPtr<double> sharedPtr8(new double(11.5));
    std::vector<pp::CustomSharedPtr<double>> sharedPtrs2;
    sharedPtrs2.push_back(sharedPtr8);
    sharedPtrs2.push_back(sharedPtr8);
    sharedPtrs2.push_back(sharedPtr8);

    const std::vector<double> expected2 = {11.5, 11.5, 11.5};
    i = 0;
    for (const auto& ptr : sharedPtrs2) {
        if (*ptr != expected2[i++]) {
            throw std::runtime_error("shared pointer failed");
        }
    }

    for (auto& ptr : sharedPtrs2) {
        ptr.release();
    }
    for (auto& ptr : sharedPtrs2) {
        if (ptr) {
            throw std::runtime_error("shared pointer failed");
        }
    }
    if (!sharedPtr8) {
        throw std::runtime_error("shared pointer failed");
    }
    sharedPtr8.release();
    if (sharedPtr8) {
        throw std::runtime_error("shared pointer failed");
    }
}

void pointersTest() {
    uniquePtrTest();

    sharedPtrTest();
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

    pointersTest();

    adapterTest();
    proxyTest();

    std::cout << "unit tests pass" << std::endl;

    return 0;
}