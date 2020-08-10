// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once
#include <cassert>
#include <map>
#include <memory>
#include <string>
#include <thread>
#include <utility>

#include "interfaces/ComponentFactory.h"

class Component;

class ComponentManager {
 private:
  static std::unique_ptr<ComponentManager> instance_;
  std::map<std::string, std::shared_ptr<ComponentFactory<Component>>>
      components_;

 public:
  template <typename T>
  static void add(std::shared_ptr<ComponentFactory<T>> factory) {
    const auto& name = factory->name();
    instance_->components_.emplace(
        name, std::reinterpret_pointer_cast<ComponentFactory<Component>>(
                  std::move(factory)));
  }

  [[nodiscard]] static const std::shared_ptr<ComponentFactory<Component>>& get(
      const std::string& name) {
    return instance_->components_[name];
  }

  static inline void create() {
    assert(((void)"ComponentManager::create() must be called only once",
            !instance_));
    instance_ = std::make_unique<ComponentManager>();
  }

  static inline void close() { instance_->components_.clear(); }
};
