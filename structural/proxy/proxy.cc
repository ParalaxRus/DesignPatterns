#include "proxy.h"

namespace patterns::structural {
 
Service::Service(std::string name) : _name(std::move(name)) { }

ServiceA::ServiceA(std::string name) : Service(std::move(name)) { }

std::string ServiceA::request() const {
    return this->_name;
}

bool Proxy::allow(const std::string& name) const {
    return (this->_allowed.find(name) != this->_allowed.end());
}

Proxy::Proxy(std::string name, std::unordered_set<std::string> allowedNames) 
    : Service(name), _allowed(std::move(allowedNames)) {
    this->_serviceA = std::make_unique<ServiceA>(name);
}

std::string Proxy::request() const {
    if (!this->allow(this->_name)) {
        return {};
    }
    return this->_serviceA->request();
}

}