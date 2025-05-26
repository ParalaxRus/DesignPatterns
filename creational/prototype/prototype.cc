#include "prototype.h"

namespace patterns::creational {

Prototype::Prototype(std::string name, int val) : _name(name), _val(val) { 
}

std::string Prototype::representation() const {
    return "Name: " + this->_name + " Val: " + std::to_string(this->_val);
}

PrototypeA::PrototypeA(std::string name, int val, int valA) 
    : Prototype(name, val), _valA(valA) { 
}

UniquePrototype PrototypeA::clone() const {
    return std::make_unique<PrototypeA>(this->_name, this->_val, this->_valA);
}

std::string PrototypeA::representation() const {
    return Prototype::representation() + " ValA: " + std::to_string(this->_valA);
}

PrototypeB::PrototypeB(std::string name, int val, int valB) 
    : Prototype(name, val), _valB(valB) { 
}

UniquePrototype PrototypeB::clone() const {
    return std::make_unique<PrototypeB>(this->_name, this->_val, this->_valB);
}

std::string PrototypeB::representation() const {
    return Prototype::representation() + " ValB: " + std::to_string(this->_valB);
}

PrototypeFactory::PrototypeFactory() {
    this->_prototypes.emplace(static_cast<int>(PrototypeType::PrototypeTypeA), 
        std::make_unique<PrototypeA>("PrototypeA", 1, 1));
    this->_prototypes.emplace(static_cast<int>(PrototypeType::PrototypeTypeB), 
        std::make_unique<PrototypeB>("PrototypeB", 2, 2));
}

UniquePrototype PrototypeFactory::create(PrototypeType protoType) const {
    auto it = this->_prototypes.find(static_cast<int>(protoType));
    if (it == this->_prototypes.end()) {
        throw std::runtime_error("");
    }
    return it->second->clone();
}

}