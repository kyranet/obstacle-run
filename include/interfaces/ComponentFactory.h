// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include <string>
#include <utility>

#include "JsonConvertible.h"
class Component;

template <typename T, typename = typename std::enable_if<
                          std::is_convertible<T, Component*>::value, T>::type>
class ComponentFactory : public JsonConvertible<T> {
  std::string name_;

 public:
  explicit ComponentFactory(std::string name) noexcept : name_(std::move(name)) {}
  virtual ~ComponentFactory() noexcept = default;
  std::string getName() const noexcept { return name_; }
};