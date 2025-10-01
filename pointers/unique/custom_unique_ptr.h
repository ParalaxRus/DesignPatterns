#pragma once

namespace patterns::pointers {

template <typename T>
class CustomUniquePtr final {
private:
    T* ptr = nullptr;

public:
    explicit CustomUniquePtr(T* ptr = nullptr) : ptr(ptr) {
    }

    // Movable
    CustomUniquePtr(CustomUniquePtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    CustomUniquePtr& operator=(CustomUniquePtr&& other) noexcept {
        if (this != &other) {
            delete this->ptr;
            this->ptr = other.ptr;
            other.ptr = nullptr;
        }
        return this;
    }

    // Not copyable
    CustomUniquePtr(const CustomUniquePtr&) = delete;
    CustomUniquePtr& operator=(const CustomUniquePtr&) = delete;

    ~CustomUniquePtr() {
        if (ptr != nullptr) {
            delete ptr;
            ptr = nullptr;
        }
    }

    const T* get() const {
        return ptr;
    }

    const T* operator ->() const {
        return this->ptr;
    }

    const T& operator *() const {
        return *this->ptr;
    }
 
    operator bool() const {
        return (get() != nullptr);
    }
};

}