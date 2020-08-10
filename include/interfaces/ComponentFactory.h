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

  virtual std::shared_ptr<T> fromJson(const Json::Value& json,
                                      std::weak_ptr<GameObject> parent) = 0;
  virtual Json::Value toJson(std::shared_ptr<T> value) const = 0;
};