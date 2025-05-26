#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace patterns::creational {

class Prototype;
using UniquePrototype = std::unique_ptr<Prototype>;

class Prototype {
protected:
    std::string _name;
    int _val;

    Prototype(std::string name, int val);

public:
    virtual ~Prototype() = default;
    virtual UniquePrototype clone() const = 0;
    virtual std::string representation() const;
};

class PrototypeA : public Prototype {
private:
    int _valA;
public:
    PrototypeA(std::string name, int val, int valA);

    UniquePrototype clone() const override;
    std::string representation() const override;
};

class PrototypeB : public Prototype {
private:
    int _valB;
public:
    PrototypeB(std::string name, int val, int valB);

    UniquePrototype clone() const override;
    std::string representation() const override;
};

enum class PrototypeType : int {
    PrototypeTypeA,
    PrototypeTypeB
};

class PrototypeFactory {
private:
    std::unordered_map<int, UniquePrototype> _prototypes;

public:
    PrototypeFactory();
    ~PrototypeFactory() = default;
    UniquePrototype create(PrototypeType protoType) const;
};

}