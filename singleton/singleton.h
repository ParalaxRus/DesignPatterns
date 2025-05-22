#pragma once

#include <memory>
#include <mutex>
#include <iostream>

namespace patterns::singleton {

class Singleton final {
private:
  static std::unique_ptr<Singleton> _singleton;
  static std::mutex _mtx;
  Singleton() = default;
  Singleton(const Singleton&) = delete;
  Singleton& operator=(const Singleton&) = delete;
  Singleton(Singleton&&) = delete;
  Singleton& operator=(Singleton&&) = delete;
public:
  static Singleton* get();
  void hi() const {
    std::cout << "Hi" << std::endl;
  }
};

}