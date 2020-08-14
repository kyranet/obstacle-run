// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include <json/json.h>

#include <string>
#include <utility>

class Component;
class GameObject;

template <typename T>
class ComponentFactory {
  std::string name_;

 public:
  explicit ComponentFactory(std::string name) noexcept
      : name_(std::move(name)) {}
  virtual ~ComponentFactory() noexcept = default;
  [[nodiscard]] inline const std::string& name() const noexcept {
    return name_;
  }

  [[nodiscard]] virtual std::shared_ptr<T> fromJson(
      const Json::Value& json,
      std::weak_ptr<GameObject> parent) const noexcept = 0;
};