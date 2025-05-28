#pragma once

#include <string>
#include <memory>
#include <unordered_set>

namespace patterns::structural {

class Service {
protected:
    std::string _name;
    Service(std::string name);

public:
    virtual ~Service() = default;
    virtual std::string request() const = 0;
};

class ServiceA : public Service {
public:
    ServiceA(std::string name);
    ~ServiceA() override = default;
    std::string request() const override;
};

using UniqueServiceA = std::unique_ptr<ServiceA>;

class Proxy : public Service {
private:
    std::unordered_set<std::string> _allowed;
    UniqueServiceA _serviceA;
    
    bool allow(const std::string& name) const;
public:
    Proxy(std::string name, std::unordered_set<std::string> allowedNames);
    ~Proxy() override = default;
    std::string request() const override;
};

}