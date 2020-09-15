// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include <json/json.h>

#include <string>
#include <utility>

#include "objects/Component.h"

class GameObject;

template <typename T,
          typename std::enable_if<std::is_base_of<Component, T>::value>::type* =
              nullptr>
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
      const Json::Value& json, std::weak_ptr<GameObject> parent) const
      noexcept {
    auto component = std::make_shared<T>(parent);
    component->patch(json);
    return component;
  }
};