#include "builder.h"

#include <iostream>

namespace patterns::creational {

void DinnerMenu::add(const std::string& item) {
    this->_menu.emplace_back(item);
}

std::string DinnerMenu::menu() const {
    std::string meal;
    for (const auto& item : this->_menu) {
        meal += item;
        meal += ";";
    }
    if (!meal.empty()) {
        meal.pop_back();
    }
    return meal;
}

void Builder::reset() {
    this->_dinner = std::make_unique<DinnerMenu>();
}

Builder::Builder() {
    this->reset();
}

UniqueDinner Builder::getDinner() {
    auto clone = std::move(this->_dinner);
    this->reset();
    return clone;
}

BbqDinnerBuilder::BbqDinnerBuilder() : Builder() { }

void BbqDinnerBuilder::makeAppetizer() const {
    this->_dinner->add("shrimp cocktail");
}

void BbqDinnerBuilder::makeEntry() const {
    this->_dinner->add("bbq ribs");
}

void BbqDinnerBuilder::makeDesert() const {
    this->_dinner->add("ice cream");
}

DinnerDirector::DinnerDirector(BbqDinnerBuilder* builder) : _builder(builder) { }

void DinnerDirector::liteDinner() {
    this->_builder->makeEntry();   
}

void DinnerDirector::fullDinner() {
    this->_builder->makeAppetizer();
    this->_builder->makeEntry();
    this->_builder->makeDesert();
}

}