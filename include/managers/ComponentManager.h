// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "interfaces/ComponentFactory.h"

class Component;

class ComponentManager {
 private:
  std::map<std::string, ComponentFactory<Component*>*> components_;
  static ComponentManager* instance_;

 public:
  template <typename T, typename = typename std::enable_if<
                            std::is_convertible<T, Component*>::value, T>::type>
  ComponentManager* add(ComponentFactory<T>* factory) {
    components_[factory->getName()] =
        reinterpret_cast<ComponentFactory<Component*>*>(factory);
    return this;
  }

  static ComponentFactory<Component*>* get(const std::string& name) {
    return getInstance()->components_[name];
  }

  static ComponentManager* getInstance() {
    if (instance_ == nullptr) instance_ = new ComponentManager();
    return instance_;
  }
};
