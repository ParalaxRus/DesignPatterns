#pragma once

namespace patterns::pointers {

template <typename T>
class CustomSharedPtr final {
private:
    T *ptr;
    int* count;
    
public:
    explicit CustomSharedPtr(T *ptr) : ptr(ptr), count(new int(1)) { }

    // Copyable
    CustomSharedPtr(const CustomSharedPtr& other) : ptr(other.ptr), count(other.count) {
        if (ptr != nullptr) {
            ++(*count);
        }
    }

    CustomSharedPtr& operator=(const CustomSharedPtr& other) {
        if (this != other) {
            release();
            ptr = other.ptr;
            count = other.count;
            if (ptr != nullptr) {
                ++(*count);
            }
        }
        return *this;
    }

    // Movable
    CustomSharedPtr(CustomSharedPtr&& other) noexcept : ptr(other.ptr), count(other.count) {
        other.ptr = nullptr;
        other.count = nullptr;
    }

    CustomSharedPtr& operator=(CustomSharedPtr&& other) noexcept {
        if (this != &other) {
            release(ptr);

            ptr = other.ptr;
            count = other.count;
        
            other.ptr = nullptr;
            other.count = nullptr;
        }
        return *this;
    }

    ~CustomSharedPtr() {
        release();
    }

    operator bool() const {
        return ptr != nullptr;
    }

    T& operator *() const {
        return *ptr;
    }

    T* operator ->() const {
        return ptr;
    }

    void release() {
        if (ptr != nullptr) {
            --(*count);
            if (*count == 0) {
                delete ptr;
                delete count;
            }
        }
        ptr = nullptr;
        count = nullptr;
    }
};

}