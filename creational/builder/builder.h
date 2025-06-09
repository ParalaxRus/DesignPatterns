#pragma once

#include <memory>
#include <string>
#include <vector>

namespace patterns::creational {

// Product
class DinnerMenu {
private:
    std::vector<std::string> _menu;

public:
    DinnerMenu() = default;
    void add(const std::string& item);
    std::string menu() const;
};

using UniqueDinner = std::unique_ptr<DinnerMenu>;

// Builder interface
class Builder {
private:
    void reset();
protected:
    Builder();
    UniqueDinner _dinner;

public:
    virtual ~Builder() = default;
    virtual void makeAppetizer() const = 0;
    virtual void makeEntry() const = 0;
    virtual void makeDesert() const = 0;
    UniqueDinner getDinner();
};

// Concete builder
class BbqDinnerBuilder : public Builder {
public:
    BbqDinnerBuilder();
    ~BbqDinnerBuilder() override = default;
    void makeAppetizer() const override;
    void makeEntry() const override;
    void makeDesert() const override;
};

// Director
class DinnerDirector {
private:
    Builder* _builder = nullptr;

public:
    DinnerDirector(BbqDinnerBuilder* builder);

    void liteDinner();
    void fullDinner();
};

}