#pragma once

#include <string>
#include <memory>

namespace patterns::singleton {

class Shape {
protected:
    Shape() = default;
public:
    virtual ~Shape() = default;
    virtual std::string draw() const = 0;
};

class Triangle : public Shape {
public:
    Triangle() = default;
    ~Triangle() override {};

    std::string draw() const override;
};

class Rectangle : public Shape {
public:
    Rectangle() = default;
    ~Rectangle() override {};
    
    std::string draw() const override;
};

using UniqueShape = std::unique_ptr<Shape>;

class Factory {
protected:
    Factory() = default;
    virtual UniqueShape create() const = 0;

public:
    virtual ~Factory() = default;
    std::string doWork() const;
};

class TriangleFactory : public Factory {
public:
    TriangleFactory() = default;
    ~TriangleFactory() override {};

    UniqueShape create() const override;
};

class RectangleFactory : public Factory {
public:
    RectangleFactory() = default;
    ~RectangleFactory() override {};

    UniqueShape create() const override;
};

}