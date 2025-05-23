#include "factorymethod.h"

namespace patterns::singleton {

std::string Triangle::draw() const {
    return "triangle";
}

std::string Rectangle::draw() const {
    return "rectangle";
}

std::string Factory::doWork() const {
    auto shape = this->create();
    return "name: " + shape->draw();
}

UniqueShape TriangleFactory::create() const {
    return std::make_unique<Triangle>();
}

UniqueShape RectangleFactory::create() const {
    return std::make_unique<Rectangle>();
}

}
